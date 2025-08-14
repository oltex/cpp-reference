#pragma once
#include "library/socket.h"
#include "library/interlock.h"
#include "library/guard.h"

#include "library/thread-local/pool.h"
#include "library/lockfree/free_list.h"
#include "library/vector.h"
#include "library/array.h"

#include "listen.h"
#include "message.h"
#include "header.h"
#include "queue.h"

#include <optional>

namespace framework {
	struct session final {
		enum class task : unsigned char {
			recv, send
		};
		struct context {
			task _task;
			library::overlap _overlap;
		};
		inline static unsigned long long _id = 0x10000;
		using size_type = unsigned int;
		unsigned long long _key;
		library::socket _socket;
		unsigned long _io_count; // release_flag // recv_flag // io count
		unsigned long _cancel_flag;

		message _receive_message;
		context _receive_context;

		unsigned long _send_flag;
		unsigned long _send_size;
		queue _send_queue;
		context _send_context;
	public:
		inline session(size_type& index) noexcept
			: _key(index++), _io_count(0x80000000) {
			_receive_context._task = task::recv;
			_send_context._task = task::send;
		};
		inline explicit session(session const&) noexcept = delete;
		inline explicit session(session&&) noexcept = delete;
		inline auto operator=(session const&) noexcept -> session & = delete;
		inline auto operator=(session&&) noexcept -> session & = delete;
		inline ~session(void) noexcept = default;

		inline void initialize(framework::accept& accept, unsigned long long timeout_duration) noexcept {
			_key = 0xffff & _key | library::interlock_exchange_add(_id, 0x10000);
			_socket = std::move(accept._socket);
			//_timeout_currnet = GetTickCount64();
			//_timeout_duration = timeout_duration;
			_cancel_flag = 0;

			_send_flag = 0;
			library::interlock_or(_io_count, 0x40000000);
			library::interlock_and(_io_count, 0x7FFFFFFF);
		}
		inline auto acquire(void) noexcept -> bool {
			auto io_count = library::interlock_increment(_io_count);
			if ((0x80000000 & io_count) || !(0x40000000 & io_count))
				return false;
			return true;
		}
		inline auto acquire(unsigned long long key) noexcept -> bool {
			auto io_count = library::interlock_increment(_io_count);
			if ((0x80000000 & io_count) || !(0x40000000 & io_count) || _key != key)
				return false;
			return true;
		}
		template<bool send = true>
		inline bool release(void) noexcept {
			if constexpr (false == send) {
				if (0x40000000 == library::interlock_and(_io_count, ~0x40000000) && 0 == library::interlock_compare_exhange(_io_count, 0x80000000, 0)) {
					_receive_message.finalize();
					while (!_send_queue.empty())
						_send_queue.pop();
					_socket.close();
					return true;
				}
			}
			else {
				if (0 == library::interlock_decrement(_io_count) && 0 == library::interlock_compare_exhange(_io_count, 0x80000000, 0)) {
					_receive_message.finalize();
					while (!_send_queue.empty())
						_send_queue.pop();
					_socket.close();
					return true;
				}
			}
			return false;
		}

		inline bool receive(void) noexcept {
			if (nullptr == _receive_message)
				_receive_message.initialize();
			else {
				framework::message message_;
				message_.initialize();
				if (!_receive_message.empty()) {
					library::memory_copy(message_.data(), _receive_message.data() + _receive_message.front(), _receive_message.size());
					message_.move_rear(_receive_message.size());
				}
				_receive_message = std::move(message_);
			}
			if (0 == _cancel_flag) {
				WSABUF wsa_buffer{ .len = _receive_message.capacity() - _receive_message.rear(), .buf = _receive_message.data() + _receive_message.rear() };
				switch (_socket.receive(wsa_buffer, _receive_context._overlap)) {
					using enum library::socket::result;
				case complet:
				case pending:
					return true;
				case close:
					return false;
				}
			}
			return false;
		}
		inline auto message(void) noexcept -> std::optional<framework::message> {
			if (sizeof(header) > _receive_message.size())
				return std::nullopt;
			header header_;
			_receive_message.peek(reinterpret_cast<unsigned char*>(&header_), sizeof(header));
			if (sizeof(header) + header_._size > _receive_message.size())
				return std::nullopt;
			_receive_message.pop(sizeof(header) + header_._size);
			return framework::message(_receive_message, _receive_message.front() - header_._size, _receive_message.front());
		}
		inline bool send(void) noexcept {
			while (!_send_queue.empty() && 0 == library::interlock_exchange(_send_flag, 1)) {
				if (_send_queue.empty())
					library::interlock_exchange(_send_flag, 0);
				else {
					WSABUF wsa_buffer[512];
					_send_size = 0;
					for (auto iter = _send_queue.begin(), end = _send_queue.end(); iter != end; ++iter, ++_send_size) {
						wsa_buffer[_send_size].buf = reinterpret_cast<char*>(iter->data() + iter->front());
						wsa_buffer[_send_size].len = iter->size();
					}
					switch (_socket.send(wsa_buffer, _send_size, _send_context._overlap)) {
						using enum library::socket::result;
					case complet:
					case pending:
						return true;
					case close:
						return false;
					}
				}
			}
			return false;
		}
		inline void flush(void) noexcept {
			for (size_type index = 0; index < _send_size; ++index)
				_send_queue.pop();
			library::interlock_exchange(_send_flag, 0);
		}

		inline void cancel(void) noexcept {
			_cancel_flag = 1;
			_socket.cancel_io_ex();
		}
		inline static auto recover(OVERLAPPED* overlapped) noexcept -> library::pair<session&, task> {
			auto context_ = reinterpret_cast<context*>(reinterpret_cast<unsigned char*>(overlapped) - offsetof(library::overlap, _overlapped) - offsetof(context, _overlap));
			if (task::recv == context_->_task)
				return library::pair<session&, task>(*reinterpret_cast<session*>(reinterpret_cast<unsigned char*>(context_) - offsetof(session, _receive_context)), context_->_task);
			else
				return library::pair<session&, task>(*reinterpret_cast<session*>(reinterpret_cast<unsigned char*>(context_) - offsetof(session, _send_context)), context_->_task);
		}
	};
	class session_array final {
		using size_type = unsigned int;
		size_type _size;
		library::lockfree::free_list<session> _free_list;
	public:
		inline explicit session_array(void) noexcept = default;
		inline explicit session_array(session_array const&) noexcept = delete;
		inline explicit session_array(session_array&&) noexcept = delete;
		inline auto operator=(session_array const&) noexcept -> session_array & = delete;
		inline auto operator=(session_array&&) noexcept -> session_array & = delete;
		inline ~session_array(void) noexcept = default;

		inline void initialize(size_type capacity) noexcept {
			size_type index = 0;
			_free_list.reserve(capacity, index);
			_size = 0;
		}
		inline void finalize(void) noexcept {
			_free_list.clear();
		}

		inline auto allocate(void) noexcept -> session* {
			auto result = _free_list.allocate();
			if (nullptr != result)
				library::interlock_increment(_size);
			return result;
		}
		inline void deallocate(session* value) noexcept {
			_free_list.deallocate(value);
			library::interlock_decrement(_size);
		}
		inline auto operator[](unsigned long long const key) noexcept -> session& {
			return _free_list[key & 0xffff];
		}
	};
}