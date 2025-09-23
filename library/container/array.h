#pragma once
#include <memory>
#include "../memory.h"
#include "../function.h"

namespace library {
	template<typename type, size_t _capacity>
	class array {
	protected:
		using size_type = unsigned int;
		using iterator = type*;
	public:
		type _array[_capacity];

		inline auto operator[](size_type const index) noexcept ->type& {
			return _array[index];
		}
		inline auto front(void) const noexcept ->type& {
			return _array[0];
		}
		inline auto back(void) const noexcept ->type& {
			return _array[_capacity - 1];
		}
		inline auto begin(void) const noexcept -> iterator {
			return _array;
		}
		inline auto end(void) const noexcept -> iterator {
			return _array + _capacity;
		}
		inline void fill(type const& value) noexcept {
			for (auto& iter : _array)
				iter = value;
		}
		inline void clear(void) noexcept {
			library::memory_set(_array, 0, sizeof(type) * _capacity);
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
		inline auto data(void) noexcept -> type* {
			return _array;
		}
		inline auto data(void) const noexcept -> type const* {
			return _array;
		}
	};
	template<typename type>
	class array<type, 0> {
	protected:
		using size_type = unsigned int;
		using iterator = type*;
		size_type _capacity;
		type* _array;
	public:
		template<typename... argument>
		inline explicit array(size_type const capacity, argument&&... arg) noexcept
			: _capacity(capacity), _array(library::allocate<type>(capacity)) {
			for (size_type index = 0; index < capacity; ++index)
				library::construct(_array[index], std::forward<argument>(arg)...);
		};
		inline explicit array(array const&) noexcept = delete;
		inline explicit array(array&& rhs) noexcept
			:_capacity(library::exchange(rhs._capacity, 0)), _array(library::exchange(rhs._array, nullptr)) {
		};
		inline auto operator=(array const&) noexcept -> array & = default;
		inline auto operator=(array&&) noexcept -> array & = default;
		inline ~array(void) noexcept {
			for (size_type index = 0; index < _capacity; ++index)
				library::destruct(_array[index]);
			library::deallocate<type>(_array);
		};

		inline auto operator[](size_type const index) noexcept ->type& {
			return _array[index];
		}
		inline auto front(void) const noexcept ->type& {
			return _array[0];
		}
		inline auto back(void) const noexcept ->type& {
			return _array[_capacity - 1];
		}
		inline auto begin(void) const noexcept -> iterator {
			return _array;
		}
		inline auto end(void) const noexcept -> iterator {
			return _array + _capacity;
		}
		inline void fill(type const& value) noexcept {
			for (auto& iter : _array)
				iter = value;
		}
		inline void clear(void) noexcept {
			library::memory_set(_array, 0, sizeof(type) * _capacity);
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
		inline auto data(void) noexcept -> type* {
			return _array;
		}
	};
}

//namespace test {
//	template<typename type, size_t _capacity>
//	class array : public library::array<type, _capacity> {
//	protected:
//		using base = library::array<type, _capacity>;
//		using size_type = base::size_type;
//		using iterator = type*;
//	public:
//
//		template<typename... argument>
//			requires (0 == _capacity)
//		inline explicit array(size_type const capacity, argument&&... arg) noexcept
//			: base(capacity, std::forward<argument>(arg)...) {
//		};
//
//		inline auto operator[](size_type const index) noexcept ->type& {
//			return base::_array[index];
//		}
//		inline auto front(void) const noexcept ->type& {
//			return base::_array[0];
//		}
//		inline auto back(void) const noexcept ->type& {
//			return base::_array[base::capacity() - 1];
//		}
//		inline auto begin(void) const noexcept -> iterator {
//			return base::_array;
//		}
//		inline auto end(void) const noexcept -> iterator {
//			return base::_array + base::capacity();
//		}
//		inline void fill(type const& value) noexcept {
//			for (auto& iter : base::_array)
//				iter = value;
//		}
//		inline void clear(void) noexcept {
//			library::memory_set(base::_array, 0, sizeof(type) * base::capacity());
//		}
//		inline auto data(void) noexcept -> type* {
//			return base::_array;
//		}
//	};
//}