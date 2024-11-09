#pragma once
#include <memory>

namespace data_structure {
	template<typename type, size_t _size>
	class array final {
	public:
		using iterator = type*;
	private:
		using size_type = unsigned int;
	public:
		inline auto operator[](size_type const idx) noexcept ->type& {
			return _arr[idx];
		}
		inline auto front(void) const noexcept ->type& {
			return _arr[0];
		}
		inline auto back(void) const noexcept ->type& {
			return _arr[_size - 1];
		}
		inline auto begin(void) const noexcept -> iterator {
			return _arr;
		}
		inline auto end(void) const noexcept -> iterator {
			return _arr + _size;
		}
	public:
		inline auto size(void) const noexcept -> size_t {
			return _size;
		}
		inline void fill(type const& value) noexcept {
			for (auto& iter : _arr)
				iter = value;
		}
		inline void clear(void) noexcept {
			memset(_arr, 0, sizeof(type) * _size);
		}
	public:
		type _arr[_size];
	};
}