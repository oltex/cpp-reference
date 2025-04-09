#pragma once
#include <memory>

namespace library::data_structure {
	class ring_buffer final {
	private:
		using byte = unsigned char;
		using size_type = unsigned int;
	public:
		inline explicit ring_buffer(void) noexcept {
			_array = reinterpret_cast<byte*>(malloc(1025));
			_capacity = 1025;
		};
		inline explicit ring_buffer(ring_buffer const& rhs) noexcept
			: _capacity(rhs._capacity), _front(rhs._front), _rear(rhs._rear) {
			_array = reinterpret_cast<byte*>(malloc(_capacity));
#pragma warning(suppress: 6387)
			memcpy(_array, rhs._array, _capacity);
		}
		inline explicit ring_buffer(ring_buffer&& rhs) noexcept
			: _capacity(rhs._capacity), _front(rhs._front), _rear(rhs._rear), _array(rhs._array) {
			rhs.clear();
			rhs._capacity = 0;
			rhs._array = nullptr;
		}
		//not implemented
		inline auto operator=(ring_buffer const& rhs) noexcept -> ring_buffer&;
		//not implemented
		inline auto operator=(ring_buffer&& rhs) noexcept -> ring_buffer&;
		inline ~ring_buffer(void) noexcept {
			free(_array);
		}
	public:
		inline auto push(byte* const buffer, size_type length) noexcept -> size_type {
			size_type rema = remain();
			if (rema < length)
				length = rema;

			size_type once = _capacity - _rear;
			if (length <= once)
				memcpy(_array + _rear, buffer, length);
			else {
				memcpy(_array + _rear, buffer, once);
				memcpy(_array, buffer + once, length - once);
			}

			_rear = (_rear + length) % _capacity;
			return length;
		};
		inline auto peek(byte* const buffer, size_type length) const noexcept -> size_type {
			size_type _size = size();
			if (_size < length)
				length = _size;

			size_type once = _capacity - _front;
			if (length <= once)
				memcpy(buffer, _array + _front, length);
			else {
				memcpy(buffer, _array + _front, once);
				memcpy(buffer + once, _array, length - once);
			}

			return length;
		};
		inline void pop(size_type length) noexcept {
			size_type _size = size();
			if (_size < length)
				length = _size;
			_front = (_front + length) % _capacity;
		}
	public:
		inline auto data(void) noexcept -> byte* {
			return _array;
		}
		inline void reserve(size_type capacity) noexcept {
			capacity += 1;
			size_type _size = size();
			if (_size < capacity) {
				byte* array_ = reinterpret_cast<byte*>(malloc(capacity));

				size_type once = _capacity - _front;
				if (_size <= once)
					memcpy(array_, _array + _front, _size);
				else {
#pragma warning(suppress: 6387)
					memcpy(array_, _array + _front, once);
					memcpy(array_ + once, _array, _size - once);
				}

				free(_array);
				_array = array_;
				_capacity = capacity;
				_front = 0;
				_rear = _size;
			}
		}
		inline void clear(void) noexcept {
			_front = _rear = 0;
		}
	public:
		inline auto size(void) const noexcept -> size_type {
			return (_rear + _capacity - _front) % _capacity;
		}
		inline auto remain(void) const noexcept -> size_type {
			return (_front + _capacity - (_rear + 1)) % _capacity;
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
		inline bool empty(void) const noexcept {
			return _front == _rear;
		}
	public:
		inline auto at_once_push(void) const noexcept -> size_type {
			size_type rema = remain();
			size_type once = _capacity - _rear;
			if (rema < once)
				return rema;
			return once;
		}
		inline auto at_once_peek(void) const noexcept -> size_type {
			size_type _size = size();
			size_type once = _capacity - _front;
			if (_size < once)
				return _size;
			return once;
		}
		inline void move_front(size_type const length) noexcept {
			_front = (_front + length) % _capacity;
		}
		inline void move_rear(size_type const length) noexcept {
			_rear = (_rear + length) % _capacity;
		}
		inline auto get_front(void) const noexcept -> size_type {
			return _front;
		}
		inline auto get_rear(void) const noexcept -> size_type {
			return _rear;
		}
	private:
		size_type _capacity = 0;
		size_type _front = 0, _rear = 0;
		byte* _array = nullptr;
	};
}