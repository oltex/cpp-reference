#pragma once
#include <malloc.h>

namespace library::system_component::memory {
	template<typename type>
	inline auto allocate(void) noexcept -> type* {
		if constexpr (16 >= alignof(type))
			return reinterpret_cast<type*>(::malloc(sizeof(type)));
		else
			return reinterpret_cast<type*>(::_aligned_malloc(sizeof(type), alignof(type)));
	}
	//reinterpret_cast<node*>(calloc(1, sizeof(node*) * 2)
	template<typename type>
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