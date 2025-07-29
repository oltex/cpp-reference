#pragma once
#pragma comment(lib, "onecore.lib")
#include <malloc.h>
#include <utility>
#include <Windows.h>

namespace library {
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
	inline auto allocate(size_t const count) noexcept -> type* {
		if constexpr (16 >= alignof(type))
			return reinterpret_cast<type*>(::malloc(sizeof(type) * count));
		else
			return reinterpret_cast<type*>(::_aligned_malloc(sizeof(type) * count, alignof(type)));
	}
	inline auto reallocate(void* pointer, size_t const size) noexcept -> void* {
		return reinterpret_cast<void*>(::realloc(pointer, size));
	}
	inline auto reallocate(void* pointer, size_t const size, size_t const align) noexcept -> void* {
		return reinterpret_cast<void*>(::_aligned_realloc(pointer, size, align));
	}
	template<typename type>
		requires (!std::is_void_v<type>)
	inline auto reallocate(type* pointer, size_t const count) noexcept -> type* {
		if constexpr (16 >= alignof(type))
			return reinterpret_cast<type*>(::realloc(pointer, sizeof(type) * count));
		else
			return reinterpret_cast<type*>(::_aligned_realloc(pointer, sizeof(type) * count, alignof(type)));
	}
	inline auto deallocate(void* const pointer) noexcept {
		::free(pointer);
	}
	inline auto deallocate(void* const pointer, size_t const align) noexcept {
		::_aligned_free(pointer);
	}
	template<typename type>
		requires (!std::is_void_v<type>)
	inline void deallocate(type* const pointer) noexcept {
		if constexpr (16 >= alignof(type))
			::free(pointer);
		else
			::_aligned_free(pointer);
	}

	inline auto memory_copy(void* const destine, void const* const source, size_t const size) noexcept -> void* {
		return ::memcpy(destine, source, size);
	}

	template<typename type, typename... argument>
	inline auto construct(type& instance, argument&&... arg) noexcept {
		if constexpr (std::is_constructible_v<type, argument...>)
			if constexpr (std::is_class_v<type>)
				::new(reinterpret_cast<void*>(&instance)) type(std::forward<argument>(arg)...);
			else if constexpr (0 < sizeof...(arg))
#pragma warning(suppress: 6011)
				instance = type(std::forward<argument>(arg)...);
	}
	template<typename type>
	inline void destruct(type& instance) noexcept {
		if constexpr (std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
			instance.~type();
	}

	inline auto get_large_page_minimum(void) noexcept -> size_t {
		return GetLargePageMinimum();
	}
	inline auto virtual_allocate(void* address, size_t size, unsigned long type, unsigned long protection) noexcept {
		return VirtualAlloc2(nullptr, address, size, type, protection, nullptr, 0);
	}
	inline auto virtual_deallocate(void* address, size_t size, unsigned long type) noexcept {
		VirtualFree(address, size, type);
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