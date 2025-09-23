#pragma once
#include "library/container/thread-local/pool.h"
#include "library/container/intrusive/share_pointer.h"
#include "library/container/array.h"
#include "library/container/lockfree/queue.h"

namespace framework {
	class message_pool;
	struct header final {
		unsigned short _size;
	};
	class buffer final : public library::intrusive::pointer_hook<0>, public library::array<char, 128> {
	public:
		inline explicit buffer(void) noexcept = delete;
		inline explicit buffer(buffer const&) noexcept = delete;
		inline explicit buffer(buffer&&) noexcept = delete;
		inline auto operator=(buffer const&) noexcept -> buffer & = delete;
		inline auto operator=(buffer&&) noexcept -> buffer & = delete;
		inline ~buffer(void) noexcept = delete;

		template<size_t index>
		inline void destructor(void) noexcept;
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
		inline void push(char* const buffer, size_type const length) noexcept {
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
		inline void peek(char* const buffer, size_type const length) const noexcept {
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
	class message_queue final : public library::lockfree::queue<message, false> {
		using base = library::lockfree::queue<message, false>;
		using size_type = unsigned int;
		size_type _size;
	public:
		inline explicit message_queue(void) noexcept
			: base(), _size(0) {
		};
		inline explicit message_queue(message_queue const&) noexcept = delete;
		inline explicit message_queue(message_queue&&) noexcept = delete;
		inline auto operator=(message_queue const&) noexcept -> message_queue & = delete;
		inline auto operator=(message_queue&&) noexcept -> message_queue & = delete;
		inline ~message_queue(void) noexcept = default;

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			library::interlock_increment(_size);
			base::emplace(std::forward<argument>(arg)...);
		}
		inline auto pop(void) noexcept -> message {
			auto result = base::pop();
			library::interlock_decrement(_size);
			return result;
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
	};
	class message_pool final : public library::_thread_local::pool<buffer>, public library::_thread_local::singleton<message_pool> {
		friend class library::_thread_local::singleton<pool>;
		using base = library::_thread_local::pool<buffer>;
		using size_type = unsigned int;
		message _message;
		inline static size_type _size = 0;

		inline explicit message_pool(void) noexcept
			: _message(base::allocate()) {
		};
		inline explicit message_pool(message_pool const&) noexcept = delete;
		inline explicit message_pool(message_pool&&) noexcept = delete;
		inline auto operator=(message_pool const&) noexcept -> message_pool & = delete;
		inline auto operator=(message_pool&&) noexcept -> message_pool & = delete;
		inline ~message_pool(void) noexcept = default;
	public:
		using library::_thread_local::singleton<pool>::instance;

		inline auto allocate(void) noexcept -> framework::message {
			library::interlock_increment(_size);
			return framework::message(base::allocate());
		}
		inline auto allocate(size_type const size) noexcept {
			if (_message.remain() < size)
				_message = allocate();
			message message(_message);
			_message.move_rear(sizeof(header) + size);
			_message.move_front(sizeof(header) + size);
			return message;
		}
		inline void deallocate(buffer* value) noexcept {
			base::deallocate(value);
			library::interlock_decrement(_size);
		}
		inline static auto size(void) noexcept -> size_type {
			return _size;
		}
	};
	template<>
	inline void buffer::destructor<0>(void) noexcept {
		message_pool::instance().deallocate(this);
	}
}