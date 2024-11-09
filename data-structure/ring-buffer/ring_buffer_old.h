#pragma once
#include <memory>

namespace data_structure {
	class ring_buffer final {
	private:
		using byte = unsigned char;
		using size_type = unsigned int;
	public:
		inline explicit ring_buffer(void) noexcept {
			_array = static_cast<byte*>(malloc(1024));
			_capacity = 1024;
		};
		inline explicit ring_buffer(ring_buffer const& rhs) noexcept
			: _size(rhs._size), _capacity(rhs._capacity), _front(rhs._front), _rear(rhs._rear) {
			_array = static_cast<byte*>(malloc(_capacity));
#pragma warning(suppress: 6387)
			memcpy(_array, rhs._array, _capacity);
		}
		inline explicit ring_buffer(ring_buffer&& rhs) noexcept
			: _size(rhs._size), _capacity(rhs._capacity), _front(rhs._front), _rear(rhs._rear), _array(rhs._array) {
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
		inline auto data(void) noexcept -> byte* {
			return _array;
		}
		inline void reserve(size_type const capacity) noexcept {
			if (_size <= capacity) {
				byte* array_ = static_cast<byte*>(malloc(capacity));

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
			_size = _front = _rear = 0;
		}
	public:
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
		inline auto remain(void) const noexcept -> size_type {
			return _capacity - _size;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
	public:
		inline auto at_once_push(void) const noexcept -> size_type {
			size_type rema = remain();	//수정 : front와 read의 위치를 먼저 비교를했으면 계산을 줄일 수 있다
			size_type once = _capacity - _rear;
			if (rema < once)
				return rema;
			return once;
		}
		inline auto at_once_peek(void) const noexcept -> size_type {
			size_type once = _capacity - _front;
			if (_size < once)
				return _size;
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
		inline auto get_front(void) const noexcept -> size_type {
			return _front;
		}
		inline auto get_rear(void) const noexcept -> size_type {
			return _rear;
		}
	private:
		size_type _size = 0;
		size_type _capacity = 0;
		size_type _front = 0, _rear = 0;
		byte* _array = nullptr;
	};
}