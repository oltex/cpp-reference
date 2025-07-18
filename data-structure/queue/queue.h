#pragma once
#include <cassert>
#include <memory>
#include <type_traits>
#include "../pool/pool.h"
#include "../vector/vector.h"
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace library {
	template<typename type, typename allocator = pool<type>, bool placement = true>
	class queue final {
		using size_type = unsigned int;
		struct node final {
			node* _next;
			type _value;
			inline explicit node(void) noexcept = delete;
			inline explicit node(node const&) noexcept = delete;
			inline explicit node(node&&) noexcept = delete;
			inline auto operator=(node const&) noexcept = delete;
			inline auto operator=(node&&) noexcept = delete;
			inline ~node(void) noexcept = delete;
		};
		size_type _size = 0;
		node* _head;
		node* _tail;
		allocator::template rebind<node> _allocator;
	public:
		inline explicit queue(void) noexcept {
			auto current = _allocator.allocate();
			current->_next = nullptr;
			_head = _tail = current;
		};
		inline explicit queue(queue const&) noexcept = delete;
		inline explicit queue(queue&& rhs) noexcept
			: queue() {
			swap(rhs);
		};
		inline auto operator=(queue const&) noexcept -> queue & = delete;
		inline auto operator=(queue&& rhs) noexcept -> queue& {
			assert(this != &rhs && "self-assignment");
			queue(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~queue(void) noexcept {
			auto current = _head->_next;
			while (nullptr != current) {
				if constexpr (true == placement)
					library::destruct(current->_value);
				_allocator.deallocate(exchange(current, current->_next));
			}
			deallocate(reinterpret_cast<void*>(_head));
		};

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			auto current = _allocator.allocate();
			current->_next = nullptr;
			if constexpr (true == placement)
				library::construct<type>(current->_value, std::forward<argument>(arg)...);
			library::exchange(_tail, current)->_next = current;
			++_size;
		}
		inline void pop(void) noexcept {
			assert(_size > 0 && "called on empty");
			auto current = library::exchange(_head->_next, _head->_next->_next);
			if (_tail == current)
				_tail = _head;
			if constexpr (true == placement)
				library::destruct(current->_value);
			_allocator.deallocate(current);
			--_size;
		}
		inline auto front(void) noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return _head->_next->_value;
		}
		inline auto back(void) noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return _tail->_value;
		}
		inline void swap(queue& rhs) noexcept {
			library::swap(_size, rhs._size);
			library::swap(_head, rhs._head);
			library::swap(_tail, rhs._tail);
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto empty(void) const noexcept -> bool {
			return 0 == _size;
		}
		inline void clear(void) noexcept {
			while (0 != _size)
				pop();
		}
	};

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
		inline auto operator=(circular_queue const&) noexcept -> circular_queue& = delete;
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
					reserve(maximum(static_cast<size_type>(capacity() * 1.5f), size() + 1));
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
		inline void reserve(size_type capacity) noexcept {
			capacity += 1;
			if (_capacity < capacity) {
				size_type size_ = size();
				size_type once = _capacity - _front;
				type* array_ = library::allocate<type>(capacity);
				if (size_ <= once)
					memcpy(array_, _array + _front, sizeof(type) * size_);
				else {
					memcpy(array_, _array + _front, sizeof(type) * once);
					memcpy(array_ + once, _array, sizeof(type) * (size_ - once));
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
			return _capacity - 1;
		}
		inline bool empty(void) const noexcept {
			return _front == _rear;
		}
		inline bool full(void) const noexcept {
			return (_rear + 1) % _capacity == _front;
		}
	};

	template<typename type, auto predicate = less<type>>
	class priority_queue final {
	private:
		using size_type = unsigned int;
	public:
		inline explicit priority_queue(void) noexcept = default;
		inline explicit priority_queue(priority_queue const&) noexcept = default;
		inline explicit priority_queue(priority_queue&&) noexcept = default;
		inline auto operator=(priority_queue const&) noexcept -> priority_queue& = default;
		inline auto operator=(priority_queue&&) noexcept -> priority_queue& = default;
		inline ~priority_queue(void) noexcept = default;

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			_vector.emplace_back(std::forward<argument>(arg)...);
			auto leaf = _vector.back();
			auto child = _vector.size() - 1;
			while (0 < child) {
				auto parent = (child - 1) / 2;

				if (predicate(_vector[parent], leaf))
					break;
				_vector[child] = _vector[parent];
				child = parent;
			}
			_vector[child] = leaf;
		};
		inline void pop(void) noexcept {
			auto leaf = _vector.back();
			auto size = _vector.size() - 1;

			size_type parent = 0;
			for (;;) {
				auto left = parent * 2 + 1;
				if (size <= left)
					break;
				auto right = left + 1;

				if (size > right && predicate(_vector[right], _vector[left]))
					left = right;
				if (predicate(leaf, _vector[left]))
					break;

				_vector[parent] = _vector[left];
				parent = left;
			}
			_vector[parent] = leaf;
			_vector.pop_back();
		}
		inline auto top(void) const noexcept -> type& {
			return _vector.front();
		};
		inline void clear(void) noexcept {
			_vector.clear();
		}
		inline void swap(priority_queue& rhs) noexcept {
			library::swap(_vector, rhs._vector);
		}
		inline auto size(void) const noexcept -> size_type {
			return _vector.size();
		}
		inline bool empty(void) const noexcept {
			return _vector.empty();
		}
	private:
		vector<type> _vector;
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