#pragma once
#include <initializer_list>
#include <utility>
#include <stdlib.h>
#include <cassert>
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace library {
	template<typename type, bool placement = true>
	class vector {
		using size_type = unsigned int;
		size_type _size;
		size_type _capacity;
		type* _array;
	public:
		using iterator = type*;

		inline explicit vector(void) noexcept
			: _size(0), _capacity(0), _array(nullptr) {
		};
		inline explicit vector(std::initializer_list<type> const& list) noexcept
			: vector() {
			reserve(list.size());
			for (auto& iter : list)
				emplace_back(iter);
		}
		inline explicit vector(iterator const& begin, iterator const& end) noexcept
			: vector() {
			reserve(end - begin);
			for (auto iter = begin; iter != end; ++iter)
				emplace_back(*iter);
		}
		inline explicit vector(vector const& rhs) noexcept
			: vector(rhs.begin(), rhs.end()) {
		}
		inline explicit vector(vector&& rhs) noexcept
			: _size(exchange(rhs._size, 0)), _capacity(exchange(rhs._capacity, 0)), _array(exchange(rhs._array, nullptr)) {
		}
		inline auto operator=(vector const& rhs) noexcept -> vector& {
			assert(this != &rhs && "self-assignment");
			vector(rhs).swap(*this);
			return *this;
		};
		inline auto operator=(vector&& rhs) noexcept -> vector& {
			assert(this != &rhs && "self-assignment");
			vector(std::move(rhs)).swap(*this);
			return *this;
		}
		inline ~vector(void) noexcept {
			clear();
			deallocate<type>(_array);
		}

		template<typename... argument>
		inline auto emplace_back(argument&&... arg) noexcept -> type& {
			if (_size >= _capacity)
				reserve(maximum(static_cast<size_type>(_capacity * 1.5f), _size + 1));
			type& element = _array[_size++];
			if constexpr (true == placement)
				construct(element, std::forward<argument>(arg)...);
			return element;
		}
		inline void pop_back(void) noexcept {
			assert(_size > 0 && "called on empty");
			--_size;
			if constexpr (true == placement)
				destruct(_array[_size]);
		}
		inline auto front(void) const noexcept ->type& {
			assert(_size > 0 && "called on empty");
			return _array[0];
		}
		inline auto back(void) const noexcept ->type& {
			assert(_size > 0 && "called on empty");
			return _array[_size - 1];
		}
		inline auto operator[](size_type const index) const noexcept ->type& {
			assert(index < _size && "index out of range");
			return _array[index];
		}
		inline auto begin(void) const noexcept -> iterator {
			return _array;
		}
		inline auto end(void) const noexcept -> iterator {
			return _array + _size;
		}

		inline void reserve(size_type const capacity) noexcept {
			if (_capacity < capacity) {
#pragma warning(suppress: 6308)
				_array = reallocate<type>(_array, capacity);
				_capacity = capacity;
			}
		}
		template<typename... argument>
		inline void resize(size_type const size, argument&&... arg) noexcept {
			if (size > _capacity)
				reserve(size);

			if (size > _size)
				while (size != _size)
					emplace_back(std::forward<argument>(arg)...);
			else
				while (size != _size)
					pop_back();
		}
		inline void assign(size_type const size, type const& value) noexcept {
			clear();
			if (size > _capacity)
				reserve(size);
			while (_size != size)
				emplace_back(value);
		}
		inline void swap(vector& rhs) noexcept {
			library::swap(_size, rhs._size);
			library::swap(_capacity, rhs._capacity);
			library::swap(_array, rhs._array);
		}
		inline void clear(void) noexcept {
			if constexpr (true == placement && std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
				while (0 != _size)
					pop_back();
			else
				_size = 0;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
		inline auto data(void) noexcept -> type* {
			return _array;
		}
	};
}