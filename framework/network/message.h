#pragma once
#include "library/thread-local/pool.h"
#include "library/intrusive/share_pointer.h"
#include "library/serialize_buffer.h"
#include "library/array.h"

namespace framework {
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
	//class memory_pool final : public library::_thread_local::singleton<memory_pool> {
	//	friend class library::_thread_local::singleton<memory_pool>;

	//	auto& pool = library::_thread_local::pool<buffer>::instance();
	//	thread_local message _message;
	//	if (nullptr == _message || _message.capacity() - _message.rear() < 10) {
	//		_message._buffer = library::_thread_local::pool<buffer>::instance().allocate();
	//	}
	//};

	class message final {
		using size_type = unsigned int;
		using share_pointer = library::intrusive::share_pointer<buffer, 0>;
		size_type _front, _rear;
		library::intrusive::share_pointer<buffer, 0> _buffer;
	public:
		inline constexpr explicit message(void) noexcept
			: _front(0), _rear(0), _buffer(nullptr) {
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

		inline auto initialize(void) noexcept {
			_front = 0;
			_rear = 0;
			_buffer = share_pointer(library::_thread_local::pool<buffer>::instance().allocate());
		}
		inline auto finalize(void) noexcept {
			_buffer = nullptr;
		}

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
}