#pragma once
#include "library/system/socket.h"
#include "library/system/interlock.h"
#include "library/container/thread-local/pool.h"
#include "library/container/lockfree/free_list.h"
#include "library/container/vector.h"
#include "library/container/array.h"
#include "message.h"
#include <optional>

namespace framework {
	class session_array;
	struct session final {
		using size_type = unsigned int;
		inline static unsigned long long _id = 0x10000;

		unsigned long long _key;
		library::socket _socket;
		unsigned long _io_count; // release_flag // recv_flag // io count
		bool _cancel_flag;

		unsigned long long _receive_timeout;
		unsigned long long _receive_expire;
		message _receive_message;
		library::overlap _receive_overlap;

		unsigned long _send_size;
		unsigned long long _send_timeout;
		unsigned long long _send_expire;
		queue _send_queue;
		library::overlap _send_overlap;
	public:
		inline session(size_type const index) noexcept
			: _key(index), _io_count(0x80000000), _receive_message(pool::instance().allocate()) {
		};
		inline explicit session(session const&) noexcept = delete;
		inline explicit session(session&&) noexcept = delete;
		inline auto operator=(session const&) noexcept -> session & = delete;
		inline auto operator=(session&&) noexcept -> session & = delete;
		inline ~session(void) noexcept = default;

		inline void initialize(framework::connection& connection, unsigned long long receive_timeout, unsigned long long send_timeout) noexcept {
			_key = 0xffff & _key | library::interlock_exchange_add(_id, 0x10000);
			_socket = std::move(connection._socket);
			_cancel_flag = 0;

			_receive_timeout = receive_timeout;
			_receive_expire = library::get_tick_count() + receive_timeout;
			_send_timeout = send_timeout;
			_send_expire = 0xFFFFFFFFFFFFFFFFULL;

			library::interlock_or(_io_count, 0x40000000);
			library::interlock_and(_io_count, 0x7FFFFFFF);
		}
		inline auto acquire_iocount(void) noexcept -> bool {
			auto io_count = library::interlock_increment(_io_count);
			return !(0x80000000 & io_count) && (0x40000000 & io_count);
		}
		inline auto acquire_iocount(unsigned long long key) noexcept -> bool {
			return acquire_iocount() && (_key == key);
		}
		inline bool release_iocount(bool receive) noexcept {
			bool flag;
			if (true == receive)
				flag = (0x40000000 == library::interlock_and(_io_count, ~0x40000000));
			else
				flag = (0 == library::interlock_decrement(_io_count));

			if (flag && 0 == library::interlock_compare_exhange(_io_count, 0x80000000, 0)) {
				while (!_send_queue.empty())
					_send_queue.pop();
				_receive_message.clear();
				_socket.close();
			}
			return flag;
		}

		inline void receive_ready(unsigned long transferred) noexcept {
			_receive_expire = library::get_tick_count64() + _receive_timeout;
			_receive_message.move_rear(transferred);
		}
		inline auto receive_message(void) noexcept -> std::optional<framework::message> {
			if (sizeof(header) > _receive_message.size())
				return std::nullopt;
			header header_;
			_receive_message.peek(reinterpret_cast<char*>(&header_), sizeof(header));
			if (sizeof(header) + header_._size > _receive_message.size())
				return std::nullopt;
			_receive_message.pop(sizeof(header) + header_._size);
			return framework::message(_receive_message, _receive_message.front() - header_._size, _receive_message.front());
		}
		inline bool receive_post(bool& receive) noexcept {
			if (1 == _cancel_flag)
				return false;
			else {
				framework::message message = pool::instance().allocate();
				if (!_receive_message.empty())
					message.push(_receive_message.data() + _receive_message.front(), _receive_message.size());
				_receive_message = std::move(message);
				WSABUF wsa_buffer{ .len = _receive_message.remain(), .buf = _receive_message.data() + _receive_message.rear() };
				auto key = _key;
				switch (_socket.receive(wsa_buffer, _receive_overlap)) {
					using enum library::socket::result;
				case pending:
					if (1 == _cancel_flag) {
						if (acquire_iocount(key))
							_socket.cancel_io_ex();
						receive = false;
						return false;
					}
				case complet:
					return true;
				case close:
					return false;
				}
			}
		}
		inline void send_finish(void) noexcept {
			for (size_type index = 0; index < _send_size; ++index)
				_send_queue.pop();
			library::interlock_exchange(_send_expire, 0xFFFFFFFFFFFFFFFFULL);
		}
		inline bool send_message(framework::message& message) noexcept {
			if (512 < _send_queue.size())
				return false;
			_send_queue.emplace(message);
			return send_post();
		}
		inline bool send_post(void) noexcept {
			if (1 == _cancel_flag)
				return false;
			while (!_send_queue.empty() && 0xFFFFFFFFFFFFFFFFULL == library::interlock_compare_exhange(_send_expire, library::get_tick_count64() + _send_timeout, 0xFFFFFFFFFFFFFFFFULL)) {
				if (_send_queue.empty())
					library::interlock_exchange(_send_expire, 0xFFFFFFFFFFFFFFFFULL);
				else {
					WSABUF wsa_buffer[512];
					_send_size = 0;
					for (auto iter = _send_queue.begin(), end = _send_queue.end(); iter != end; ++iter, ++_send_size) {
						wsa_buffer[_send_size].buf = reinterpret_cast<char*>(iter->data() + iter->front());
						wsa_buffer[_send_size].len = iter->size();
					}
					auto key = _key;
					switch (_socket.send(wsa_buffer, _send_size, _send_overlap)) {
						using enum library::socket::result;
					case pending:
						if (1 == _cancel_flag) {
							if (acquire_iocount(key))
								_socket.cancel_io_ex();
							return false;
						}
					case complet:
						return true;
					case close:
						return false;
					}
				}
			}
			return false;
		}
		inline void cancel_network(void) noexcept {
			_cancel_flag = true;
			_socket.cancel_io_ex();
		}
	};
	class session_array final : public library::lockfree::free_list<session, false, false> {
		using size_type = unsigned int;
		using base = library::lockfree::free_list<session, false, false>;
		size_type _size;
	public:
		inline explicit session_array(size_type const capacity) noexcept
			: base(capacity), _size(0) {
			for (size_type index = 0; index < capacity; ++index)
				library::construct(_array[index]._value, index);
		};
		inline explicit session_array(session_array const&) noexcept = delete;
		inline explicit session_array(session_array&&) noexcept = delete;
		inline auto operator=(session_array const&) noexcept -> session_array & = delete;
		inline auto operator=(session_array&&) noexcept -> session_array & = delete;
		inline ~session_array(void) noexcept {
			for (size_type index = 0; index < _capacity; ++index)
				library::destruct(_array[index]._value);
		};

		inline auto allocate(void) noexcept -> session* {
			auto result = base::allocate();
			if (nullptr != result)
				library::interlock_increment(_size);
			return result;
		}
		inline void deallocate(session* value) noexcept {
			base::deallocate(value);
			library::interlock_decrement(_size);
		}
		inline auto operator[](unsigned long long const key) noexcept -> session& {
			return base::operator[](key & 0xffff);
		}
		inline auto operator[](OVERLAPPED* overlapped) noexcept -> session& {
			auto offset = reinterpret_cast<char*>(overlapped) - reinterpret_cast<char*>(base::_array);
			auto index = static_cast<size_type>(offset / sizeof(base::node));
			return base::operator[](index);
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
	};
}