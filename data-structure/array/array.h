#pragma once
#include <memory>
#include "../../memory/memory.h"

namespace library {
	template<typename type, size_t _size>
	class array {
	protected:
		using size_type = unsigned int;
		using iterator = type*;
		type _array[_size];
	public:
		inline explicit array(void) noexcept = default;
		inline explicit array(array const& rhs) noexcept = default;
		inline explicit array(array&& rhs) noexcept = default;
		inline auto operator=(array const& rhs) noexcept -> array & = default;
		inline auto operator=(array&& rhs) noexcept -> array & = default;
		inline ~array(void) noexcept = default;

		inline auto operator[](size_type const index) noexcept ->type& {
			return _array[index];
		}
		inline auto front(void) const noexcept ->type& {
			return _array[0];
		}
		inline auto back(void) const noexcept ->type& {
			return _array[_size - 1];
		}
		inline auto begin(void) const noexcept -> iterator {
			return _array;
		}
		inline auto end(void) const noexcept -> iterator {
			return _array + _size;
		}
		inline auto size(void) const noexcept -> size_t {
			return _size;
		}
		inline void fill(type const& value) noexcept {
			for (auto& iter : _array)
				iter = value;
		}
		inline void clear(void) noexcept {
			library::memory_set(_array, 0, sizeof(type) * _size);
		}
		inline auto data(void) noexcept -> type* {
			return _array;
		}
	};
}