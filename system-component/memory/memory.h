#pragma once
#include <malloc.h>
#include <utility>

namespace library::system_component::memory {
	inline auto allocate(size_t const size) noexcept -> void* {
		return reinterpret_cast<void*>(::malloc(size));
	}
	inline auto allocate(size_t const size, size_t const align) noexcept -> void* {
		return reinterpret_cast<void*>(::_aligned_malloc(size, align));
	}
	template<typename type>
		requires (!std::is_void_v<type>)
	inline auto allocate(void) noexcept -> type* {
		if constexpr (16 >= alignof(type))
			return reinterpret_cast<type*>(::malloc(sizeof(type)));
		else
			return reinterpret_cast<type*>(::_aligned_malloc(sizeof(type), alignof(type)));
	}
	template<typename type>
		requires (!std::is_void_v<type>)
	inline auto allocate(size_t const size) noexcept -> type* {
		if constexpr (16 >= alignof(type))
			return reinterpret_cast<type*>(::malloc(sizeof(type) * size));
		else
			return reinterpret_cast<type*>(::_aligned_malloc(sizeof(type) * size, alignof(type)));
	}
	inline auto deallocate(void* const value) noexcept {
		::free(value);
	}
	template<typename type>
		requires (!std::is_void_v<type>)
	inline auto deallocate(type* const value) noexcept {
		if constexpr (16 >= alignof(type))
			::free(value);
		else
			::_aligned_free(value);
	}
	template<typename type, typename... argument>
	inline auto construct(type& value, argument&&... arg) noexcept {
		if constexpr (std::is_constructible_v<type, argument...>)
			if constexpr (std::is_class_v<type>)
				::new(reinterpret_cast<void*>(&value)) type(std::forward<argument>(arg)...);
			else
#pragma warning(suppress: 6011)
				value = type(std::forward<argument>(arg)...);
	}
	template<typename type>
	inline auto destruct(type& value) noexcept {
		if constexpr (std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
			value.~type();
	}
}

//if constexpr (!std::is_trivially_constructible_v<type, argument...>)

//template<typename type>
//inline auto allocate(size_t const number) noexcept -> type* {
//	if constexpr (16 >= alignof(type))
//		return reinterpret_cast<type*>(::calloc(number, sizeof(type)));
//	else
//		return reinterpret_cast<type*>(::_aligned_ca(sizeof(type), alignof(type)));
//	reinterpret_cast<node*>(::calloc(1, sizeof(node*) * 2)
//}