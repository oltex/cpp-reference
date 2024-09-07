#pragma once
#include <memory>

namespace data_structure {
	class ring_buffer final {
		using byte = unsigned char;
		using size_type = unsigned int;
	public:
		inline explicit ring_buffer(void) noexcept {
			reserve(1024);
		};
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
			_size += length;
			return length;
		};
		inline auto peek(byte* const buffer, size_type length) const noexcept -> size_type {
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
			if (_size < length)
				length = _size;
			_front = (_front + length) % _capacity;
			_size -= length;
		}
	public:
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
		inline auto remain(void) const noexcept -> size_type {
			return _capacity - _size;
		}
		inline void clear(void) noexcept {
			_size = _front = _rear = 0;
		}
		inline void reserve(size_type const capacity) noexcept {
			if (_size > capacity)
				return;
			byte* array_ = static_cast<byte*>(malloc(sizeof(byte) * capacity));

			size_type copy = (_capacity - _front);
			copy = _size < copy ? _size : copy;

#pragma warning(suppress: 6387)
			memcpy(array_, _array + _front, sizeof(byte) * copy);
			if (0 != _size - copy)
				memcpy(array_ + copy, _array, sizeof(byte) * (_size - copy));

			free(_array);
			_array = array_;
			_capacity = capacity;
			_front = 0;
			_rear = _size;
		}
		inline auto data(void) noexcept -> byte* {
			return _array;
		}
	public:
		inline auto at_once_send(void) noexcept -> size_type {
			size_type once = _capacity - _front;
			if (_size < once)
				once = _size;
			return once;
		}
		inline auto at_once_receive(void) noexcept -> size_type {
			size_type rema = remain();
			size_type once = _capacity - _rear;
			if (rema < once)
				once = rema;
			return once;
		}
		inline void move_front(size_type const length) noexcept {
			_front = (_front + length) % _capacity;
			_size -= length;
		}
		inline void move_rear(size_type const length) noexcept {
			_rear = (_rear + length) % _capacity;
			_size += length;
		}
		inline auto get_front(void) noexcept -> size_type {
			return _front;
		}
		inline auto get_rear(void) noexcept -> size_type {
			return _rear;
		}
	private:
		size_type _size = 0;
		size_type _capacity = 0;
		size_type _front = 0, _rear = 0;
		byte* _array = nullptr;
	};
}