#pragma once
#include "library/container/thread-local/pool.h"
#include "library/container/intrusive/share_pointer.h"
#include "library/container/array.h"
#include "library/container/lockfree/queue.h"

namespace framework {
	struct header final {
		unsigned short _size;
		inline explicit header(void) noexcept = default;
		inline explicit header(header const&) noexcept = delete;
		inline explicit header(header&&) noexcept = delete;
		inline auto operator=(header const&) noexcept -> header & = delete;
		inline auto operator=(header&&) noexcept -> header & = delete;
		inline ~header(void) noexcept = default;
	};

	class buffer : public library::intrusive::share_pointer_hook<0>, public library::array<char, 128> {
		using byte = unsigned char;
	public:
		inline explicit buffer(void) noexcept = delete;
		inline explicit buffer(buffer const&) noexcept = delete;
		inline explicit buffer(buffer&&) noexcept = delete;
		inline auto operator=(buffer const&) noexcept -> buffer & = delete;
		inline auto operator=(buffer&&) noexcept -> buffer & = delete;
		inline ~buffer(void) noexcept = delete;

		template<size_t index>
		inline void destructor(void) noexcept;
		template<>
		inline void destructor<0>(void) noexcept {
			library::_thread_local::pool<buffer>::instance().deallocate(this);
		}
	};

	class message final {
		using size_type = unsigned int;
		using share_pointer = library::intrusive::share_pointer<buffer, 0>;
		size_type _front, _rear;
		library::intrusive::share_pointer<buffer, 0> _buffer;
	public:
		inline constexpr explicit message(void) noexcept
			: _front(0), _rear(0), _buffer(nullptr) {
		}
		inline constexpr message(nullptr_t) noexcept
			: _front(0), _rear(0), _buffer(nullptr) {
		}
		inline explicit message(buffer* buffer) noexcept
			: _front(0), _rear(0), _buffer(buffer) {
		}
		inline explicit message(message& rhs, size_type front, size_type rear) noexcept
			: _front(front), _rear(rear), _buffer(rhs._buffer) {
		}
		inline explicit message(message const& rhs) noexcept
			: _front(rhs._front), _rear(rhs._rear), _buffer(rhs._buffer) {
		};
		inline message(message&& rhs) noexcept
			: _front(library::exchange(rhs._front, 0)), _rear(library::exchange(rhs._rear, 0)), _buffer(std::move(rhs._buffer)) {
		};
		inline auto operator=(message const& rhs) noexcept -> message& {
			_front = rhs._front;
			_rear = rhs._rear;
			_buffer = rhs._buffer;
		};
		inline auto operator=(message&& rhs) noexcept -> message& {
			_front = library::exchange(rhs._front, 0);
			_rear = library::exchange(rhs._rear, 0);
			_buffer = std::move(rhs._buffer);
			return *this;
		};
		inline ~message(void) noexcept = default;

		template<typename type>
			requires library::arithmetic_type<type>
		inline auto operator<<(type const value) noexcept -> message& {
			assert(sizeof(type) + _rear <= _buffer->capacity() && "not enough capacity");
			reinterpret_cast<type&>((*_buffer)[_rear]) = value;
			_rear += sizeof(type);
			return *this;
		}
		inline void push(byte* const buffer, size_type const length) noexcept {
			assert(length + _rear <= _buffer->capacity() && "not enough capacity");
			library::memory_copy(_buffer->data() + _rear, buffer, length);
			_rear += length;
		}
		template<typename type>
			requires library::arithmetic_type<type>
		inline auto operator>>(type& value) noexcept -> message& {
			value = reinterpret_cast<type&>((*_buffer)[_front]);
			_front += sizeof(type);
			return *this;
		}
		inline void peek(byte* const buffer, size_type const length) const noexcept {
			assert(length + _front <= _rear && "not enough data");
			library::memory_copy(buffer, _buffer->data() + _front, length);
		}
		inline void pop(size_type const length) noexcept {
			assert(length + _front <= _rear && "not enough data");
			_front += length;
		}

		inline auto capacity(void) const noexcept -> size_type {
			return _buffer->capacity();
		}
		inline auto remain(void) noexcept -> size_type {
			return _buffer->capacity() - _rear;
		}
		inline void clear(void) noexcept {
			_front = _rear = 0;
		}
		inline bool empty(void) const noexcept {
			return _front == _rear;
		}
		inline auto size(void) const noexcept -> size_type {
			return _rear - _front;
		}
		inline auto front(void) const noexcept -> size_type {
			return _front;
		}
		inline auto rear(void) const noexcept -> size_type {
			return _rear;
		}
		inline void move_front(size_type const length) noexcept {
			_front += length;
		}
		inline void move_rear(size_type const length) noexcept {
			_rear += length;
		}
		inline auto data(void) noexcept {
			return _buffer->data();
		}
		inline friend bool operator==(message const& lhs, nullptr_t) noexcept {
			return nullptr == lhs._buffer;
		}
	};

	class pool final : public library::_thread_local::singleton<pool> {
		friend class library::_thread_local::singleton<pool>;
		using size_type = unsigned int;
		message _message;

		inline explicit pool(void) noexcept = default;
		inline explicit pool(pool const&) noexcept = delete;
		inline explicit pool(pool&&) noexcept = delete;
		inline auto operator=(pool const&) noexcept -> pool & = delete;
		inline auto operator=(pool&&) noexcept -> pool & = delete;
		inline ~pool(void) noexcept = default;
	public:
		inline auto allocate(void) noexcept {
			auto& instance = library::_thread_local::pool<buffer>::instance();
			return framework::message(instance.allocate());
		}
		inline auto allocate(size_type const size) noexcept {
			if (nullptr == _message || _message.remain() < sizeof(header) + size) {
				auto& instance = library::_thread_local::pool<buffer>::instance();
				_message = framework::message(instance.allocate());
			}
			message message(_message);
			_message.move_rear(sizeof(header) + size);
			_message.move_front(sizeof(header) + size);
			return message;
		}
	};

	class queue final : public library::lockfree::queue<message, false> {
	public:
		inline explicit queue(void) noexcept = default;
		inline explicit queue(queue const&) noexcept = delete;
		inline explicit queue(queue&&) noexcept = delete;
		inline auto operator=(queue const&) noexcept -> queue & = delete;
		inline auto operator=(queue&&) noexcept -> queue & = delete;
		inline ~queue(void) noexcept = default;
	};
}