#pragma once
#include <memory>
#include <type_traits>

namespace data_structure {
	template<typename type>
	class circular_queue final {
	private:
		using size_type = unsigned int;
		class iterator final {
		public:
			inline explicit iterator(type* const array_, size_type current, size_type capacity) noexcept
				: _array(array_), _current(current), _capacity(capacity) {
			}
			inline iterator(iterator const& rhs) noexcept
				: _array(rhs._array), _current(rhs._current), _capacity(rhs._capacity) {
			}
			inline auto operator=(iterator const& rhs) noexcept -> iterator& {
				_array = rhs._array;
				_current = rhs._current;
				_capacity = rhs._capacity;
				return *this;
			}
		public:
			inline auto operator*(void) const noexcept -> type& {
				return _array[_current];
			}
			inline auto operator++(void) noexcept -> iterator& {
				_current = (_current + 1) % _capacity;
				return *this;
			}
			inline auto operator++(int) noexcept -> iterator {
				iterator iter(*this);
				_current = (_current + 1) % _capacity;
				return iter;
			}
			inline auto operator--(void) noexcept -> iterator& {
				_front = (_front + 1) % _capacity;
				return *this;
			}
			inline auto operator--(int) noexcept -> iterator {
				iterator iter(*this);
				_front = (_front + 1) % _capacity;
				return iter;
			}
			inline bool operator==(iterator const& rhs) const noexcept {
				return _current == rhs._current;
			}
			inline bool operator!=(iterator const& rhs) const noexcept {
				return _current != rhs._current;
			}
		public:
			size_type _capacity = 0;
			size_type _current = 0;
			type* _array;
		};
	public:
		inline explicit circular_queue(void) noexcept {
			_array = static_cast<type*>(malloc(sizeof(type) * 11));
			_capacity = 11;
		};
		inline explicit circular_queue(circular_queue const& rhs) noexcept
			: _capacity(rhs._capacity), _front(rhs._front), _rear(rhs._rear) {
			_array = reinterpret_cast<type*>(malloc(sizeof(type) * _capacity));
#pragma warning(suppress: 6387)
			memcpy(_array, rhs._array, _capacity);
		};
		inline explicit circular_queue(circular_queue&& rhs) noexcept
			: _capacity(rhs._capacity), _front(rhs._front), _rear(rhs._rear), _array(rhs._array) {
			rhs.clear();
			rhs._capacity = 0;
			rhs._array = nullptr;
		};
		//not implemented
		inline auto operator=(circular_queue const& rhs) noexcept -> circular_queue&;
		//not implemented
		inline auto operator=(circular_queue&& rhs) noexcept -> circular_queue&;
		inline ~circular_queue(void) noexcept {
			if constexpr (!std::is_trivially_destructible_v<type>)
				while (!empty()) {
					_array[_front].~type();
					_front = (_front + 1) % _capacity;
				}
			free(_array);
		}
	public:
		template<typename universal>
		inline void push(universal&& value) noexcept {
			emplace(std::forward<universal>(value));
		};
		template<typename... argument>
		inline auto emplace(argument&&... arg) noexcept -> type& {
			//if (full()) {
			//	size_type capacity = static_cast<size_type>(_capacity * 1.5f);
			//	if (size() + 1 >= capacity)
			//		++capacity;
			//	reserve(capacity);
			//}
			type* element = _array + _rear;
			if (!full()) {
				if constexpr (std::is_class_v<type>) {
					if constexpr (std::is_constructible_v<type, argument...>)
						new(element) type(std::forward<argument>(arg)...);
				}
				else if constexpr (1 == sizeof...(arg))
#pragma warning(suppress: 6011)
					* element = type(std::forward<argument>(arg)...);
				_rear = (_rear + 1) % _capacity;
			}
			return *element;
		}
		inline void pop(void) noexcept {
			if (!empty()) {
				if constexpr (!std::is_trivially_destructible_v<type>)
					_array[_front].~type();
				_front = (_front + 1) % _capacity;
			}
		}
	public:
		inline auto top(void) const noexcept -> type& {
			return _array[_front];
		};
		inline auto begin(void) noexcept -> iterator {
			return iterator(_array, _front, _capacity);
		}
		inline auto end(void) noexcept -> iterator {
			return iterator(_array, _rear, _capacity);
		}
	public:
		inline void clear(void) noexcept {
			if constexpr (!std::is_trivially_destructible_v<type>)
				while (!empty())
					pop();
			else
				_front = _rear;
		}
		inline void reserve(size_type& capacity) noexcept {
			capacity += 1;
			size_type _size = size();
			if (_size < capacity) {
				type* array_ = reinterpret_cast<type*>(malloc(sizeof(type) * capacity));

				size_type once = _capacity - _front;
				if (_size <= once)
					memcpy(array_, _array + _front, sizeof(type) * _size);
				else {
					memcpy(array_, _array + _front, sizeof(type) * once);
					memcpy(array_ + once, _array, sizeof(type) * (_size - once));
				}

				free(_array);
				_array = array_;
				_capacity = capacity;
				_front = 0;
				_rear = _size;
			}
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
		inline bool full(void) const noexcept {
			return (_rear + 1) % _capacity == _front;
		}
	private:
		size_type _capacity = 0;
		size_type _front = 0, _rear = 0;
		type* _array = nullptr;
	};
}