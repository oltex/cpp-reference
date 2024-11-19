#pragma once
#include <memory>
#include <type_traits>

namespace data_structure {
	template<typename type>
	class circular_queue final {
	private:
		using size_type = unsigned int;
	public:
		inline explicit circular_queue(void) noexcept = default;
		//not implemented
		inline explicit circular_queue(circular_queue const& rhs) noexcept;
		//not implemented
		inline explicit circular_queue(circular_queue&& rhs) noexcept;
		//not implemented
		inline auto operator=(circular_queue const& rhs) noexcept -> circular_queue&;
		//not implemented
		inline auto operator=(circular_queue&& rhs) noexcept -> circular_queue&;
		inline ~circular_queue(void) noexcept {
			clear();
			free(_array);
		}
	public:
		template<typename universal>
		inline void push(universal&& value) noexcept {
			emplace(std::forward<universal>(value));
		};
		template<typename... argument>
		inline auto emplace(argument&&... arg) noexcept -> type& {
			if (_size >= _capacity) {
				size_type capacity = static_cast<size_type>(_capacity * 1.5f);
				if (_size >= capacity)
					++capacity;
				reserve(capacity);
			}
			type* element = _array + _rear;
			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					new(element) type(std::forward<argument>(arg)...);
			}
			else if constexpr (1 == sizeof...(arg))
#pragma warning(suppress: 6011)
				* element = type(std::forward<argument>(arg)...);
			_rear = (_rear + 1) % _capacity;
			++_size;
			return *element;
		}
		inline void pop(void) noexcept {
			if (!empty()) {
				if constexpr (!std::is_trivially_destructible_v<type>)
					_array[_front].~type();
				_front = (_front + 1) % _capacity;
				--_size;
			}
		}
	public:
		inline auto top(void) const noexcept -> type& {
			return _array[_front];
		};
	public:
		inline void clear(void) noexcept {
			if constexpr (!std::is_trivially_destructible_v<type>) {
				while (0 != _size)
					pop();
			}
			else {
				_front = _rear;
				_size = 0;
			}
		}
		inline void reserve(size_type const& capacity) noexcept {
			if (_size <= capacity) {
				type* array_ = reinterpret_cast<type*>(malloc(sizeof(type) * capacity));

				//size_type copy = (_capacity - _front);
				//copy = _size < copy ? _size : copy;
				//memcpy(array_, _array + _front, sizeof(type) * copy);
				//memcpy(array_ + copy, _array, sizeof(type) * (_size - copy));
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
			return _size;
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
	private:
		size_type _size = 0;
		size_type _capacity = 0;
		size_type _front = 0, _rear = 0;
		type* _array = nullptr;
	};
}