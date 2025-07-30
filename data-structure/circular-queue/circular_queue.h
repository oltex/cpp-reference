#pragma once
#include <cassert>
#include <memory>
#include <type_traits>
#include "../pool/pool.h"
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace library {
	template<typename type, bool resize = true, bool placement = true>
	class circular_queue final {
		using size_type = unsigned int;
		size_type _capacity;
		size_type _front, _rear;
		type* _array;
	public:
		inline explicit circular_queue(void) noexcept
			: _capacity(1), _front(0), _rear(0), _array(library::allocate<type>(1)) {
		};
		inline explicit circular_queue(circular_queue const&) noexcept = delete;
		inline explicit circular_queue(circular_queue&& rhs) noexcept
			: circular_queue() {
			swap(rhs);
		};
		inline auto operator=(circular_queue const&) noexcept -> circular_queue & = delete;
		inline auto operator=(circular_queue&& rhs) noexcept -> circular_queue& {
			circular_queue(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~circular_queue(void) noexcept {
			clear();
			library::deallocate<type>(_array);
		}

		template<typename... argument>
		inline auto emplace(argument&&... arg) noexcept -> type& {
			if constexpr (true == resize) {
				if (full())
					reserve(maximum(static_cast<size_type>(_capacity * 1.5f), size() + 2));
			}
			else
				assert(full() && "called on full");
			type& element = _array[_rear];
			if constexpr (true == placement)
				library::construct<type>(element, std::forward<argument>(arg)...);
			_rear = (_rear + 1) % _capacity;
			return element;
		}
		inline void pop(void) noexcept {
			assert(!empty() && "called on empty");
			if constexpr (true == placement)
				library::destruct<type>(_array[_front]);
			_front = (_front + 1) % _capacity;
		}
		inline auto top(void) const noexcept -> type& {
			assert(!empty() && "called on empty");
			return _array[_front];
		};
		inline void clear(void) noexcept {
			if constexpr (true == placement && std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
				while (!empty())
					pop();
			else
				_front = _rear;
		}
		inline void swap(circular_queue& rhs) noexcept {
			library::swap(_capacity, rhs._capacity);
			library::swap(_front, rhs._front);
			library::swap(_rear, rhs._rear);
			library::swap(_array, rhs._array);
		}
		inline void reserve(size_type const capacity) noexcept {
			if (_capacity < capacity) {
				size_type size_ = size();
				size_type once = _capacity - _front;
				type* array_ = library::allocate<type>(capacity);
				if (size_ <= once)
					library::memory_copy(array_, _array + _front, size_);
				else {
					library::memory_copy(array_, _array + _front, once);
					library::memory_copy(array_ + once, _array, size_ - once);
				}
				library::deallocate<type>(_array);

				_capacity = capacity;
				_front = 0;
				_rear = size_;
				_array = array_;
			}
		}
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
	};
}

//class iterator final {
//public:
//	inline explicit iterator(type* const array_, size_type current, size_type capacity) noexcept
//		: _array(array_), _current(current), _capacity(capacity) {
//	}
//	inline iterator(iterator const& rhs) noexcept
//		: _array(rhs._array), _current(rhs._current), _capacity(rhs._capacity) {
//	}
//	inline auto operator=(iterator const& rhs) noexcept -> iterator& {
//		_array = rhs._array;
//		_current = rhs._current;
//		_capacity = rhs._capacity;
//		return *this;
//	}
//public:
//	inline auto operator*(void) const noexcept -> type& {
//		return _array[_current];
//	}
//	inline auto operator->(void) const noexcept -> type* {
//		return _array + _current;
//	}
//	inline auto operator++(void) noexcept -> iterator& {
//		_current = (_current + 1) % _capacity;
//		return *this;
//	}
//	inline auto operator++(int) noexcept -> iterator {
//		iterator iter(*this);
//		_current = (_current + 1) % _capacity;
//		return iter;
//	}
//	inline auto operator--(void) noexcept -> iterator& {
//		_front = (_front + 1) % _capacity;
//		return *this;
//	}
//	inline auto operator--(int) noexcept -> iterator {
//		iterator iter(*this);
//		_front = (_front + 1) % _capacity;
//		return iter;
//	}
//	inline bool operator==(iterator const& rhs) const noexcept {
//		return _current == rhs._current;
//	}
//	inline bool operator!=(iterator const& rhs) const noexcept {
//		return _current != rhs._current;
//	}
//public:
//	size_type _capacity = 0;
//	size_type _current = 0;
//	type* _array;
//};
//inline auto begin(void) noexcept -> iterator {
//	return iterator(_array, _front, _capacity);
//}
//inline auto end(void) noexcept -> iterator {
//	return iterator(_array, _rear, _capacity);
//}