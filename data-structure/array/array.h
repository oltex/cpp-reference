#pragma once
#include <memory>
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace detail {
	template<typename type, size_t _capacity>
	class array {
	protected:
		using size_type = unsigned int;
		using iterator = type*;
		type _array[_capacity];
	public:
		inline explicit array(void) noexcept = default;
		inline explicit array(array const& rhs) noexcept = default;
		inline explicit array(array&& rhs) noexcept = default;
		inline auto operator=(array const& rhs) noexcept -> array & = default;
		inline auto operator=(array&& rhs) noexcept -> array & = default;
		inline ~array(void) noexcept = default;

		inline static auto capacity(void) noexcept -> size_type {
			return _capacity;
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
		inline explicit array(void) noexcept
			: _capacity(0), _array(nullptr) {
		};
		inline explicit array(array const& rhs) noexcept
			: _capacity(rhs._capacity), _array(library::allocate<type>(_capacity)) {
		};
		inline explicit array(array&& rhs) noexcept
			:_capacity(library::exchange(rhs._capacity, 0)), _array(library::exchange(rhs._array, nullptr)) {
		};
		inline auto operator=(array const& rhs) noexcept -> array & = default;
		inline auto operator=(array&& rhs) noexcept -> array & = default;
		inline ~array(void) noexcept = default;

		inline void allocate(size_type const capacity) noexcept {
			_array = library::allocate<type>(capacity);
			_capacity = capacity;
		}
		inline void deallocate(void) noexcept {
			library::deallocate<type>(_array);
		}
		inline void reallocate(void) noexcept {
			_array = library::reallocate<type>(_array, capacity);
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
	};
}
//template<typename... argument>
//for (size_type index = 0; index < capacity; ++index)
//	library::construct(_array[index], std::forward<argument>(arg)...);

namespace library {
	template<typename type, size_t _capacity>
	class array : public detail::array<type, _capacity> {
	protected:
		using base = detail::array<type, _capacity>;
		using size_type = base::size_type;
		using iterator = type*;
	public:
		inline explicit array(void) noexcept = default;
		inline explicit array(array const&) noexcept = default;
		inline explicit array(array&&) noexcept = default;
		inline auto operator=(array const&) noexcept -> array & = default;
		inline auto operator=(array&&) noexcept -> array & = default;
		inline ~array(void) noexcept = default;

		inline auto operator[](size_type const index) noexcept ->type& {
			return base::_array[index];
		}
		inline auto front(void) const noexcept ->type& {
			return base::_array[0];
		}
		inline auto back(void) const noexcept ->type& {
			return base::_array[base::capacity() - 1];
		}
		inline auto begin(void) const noexcept -> iterator {
			return base::_array;
		}
		inline auto end(void) const noexcept -> iterator {
			return base::_array + base::capacity();
		}
		inline void fill(type const& value) noexcept {
			for (auto& iter : base::_array)
				iter = value;
		}
		inline void clear(void) noexcept {
			library::memory_set(base::_array, 0, sizeof(type) * base::capacity());
		}
		inline auto data(void) noexcept -> type* {
			return base::_array;
		}
	};
}