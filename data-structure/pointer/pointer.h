#pragma once
#include <malloc.h>
#include <utility>
#include <type_traits>
#include "../../memory/memory.h"

namespace library {
	template<typename type>
	class unique_pointer final {
		type* _pointer;
	public:
		inline constexpr explicit unique_pointer(void) noexcept
			: _pointer(nullptr) {
		}
		inline constexpr unique_pointer(nullptr_t) noexcept
			: _pointer(nullptr) {
		};
		inline unique_pointer(type* pointer) noexcept
			: _pointer(pointer) {
		}
		template<typename... argument>
		inline explicit unique_pointer(argument&&... arg) noexcept {
			_pointer = system::memory::allocate<type>();
			system::memory::construct<type>(*_pointer, std::forward<argument>(arg)...);
		}
		inline unique_pointer(unique_pointer&) noexcept = delete;
		inline explicit unique_pointer(unique_pointer&& rhs) noexcept
			: _pointer(rhs._pointer) {
			rhs._pointer = nullptr;
		};
		inline auto operator=(unique_pointer const&) noexcept -> unique_pointer & = delete;
		inline auto operator=(unique_pointer&& rhs) noexcept -> unique_pointer& {
			_pointer = rhs._pointer;
			rhs._pointer = nullptr;
			return *this;
		};
		inline ~unique_pointer(void) noexcept {
			if (nullptr != _pointer) {
				system::memory::destruct<type>(*_pointer);
				system::memory::deallocate<type>(_pointer);
			}
		}

		inline auto operator*(void) noexcept -> type& {
			return *_pointer;
		}
		inline auto operator->(void) noexcept -> type* {
			return _pointer;
		}
		inline void set(type* value) noexcept {
			_pointer = value;
		}
		inline void reset(void) noexcept {
			_pointer = nullptr;
		}
		inline auto get(void) const noexcept -> type* {
			return _pointer;
		}
		template <class type>
		friend inline bool operator==(unique_pointer<type> const& value, nullptr_t) noexcept {
			return value._pointer == nullptr;
		}
	};

	template<typename type>
	class unique_pointer<type[]> final {
		using size_type = unsigned int;
		type* _pointer;
	public:
		inline constexpr explicit unique_pointer(void) noexcept
			: _pointer(nullptr) {
		}
		inline constexpr unique_pointer(nullptr_t) noexcept
			: _pointer(nullptr) {
		};
		inline unique_pointer(type* pointer) noexcept
			: _pointer(pointer) {
		}
		inline unique_pointer(unique_pointer&) noexcept = delete;
		inline explicit unique_pointer(unique_pointer&& rhs) noexcept
			: _pointer(rhs._pointer) {
			rhs._pointer = nullptr;
		};
		inline auto operator=(unique_pointer const&) noexcept -> unique_pointer & = delete;
		inline auto operator=(unique_pointer&& rhs) noexcept -> unique_pointer& {
			_pointer = rhs._pointer;
			rhs._pointer = nullptr;
			return *this;
		};
		inline ~unique_pointer(void) noexcept {
			if (nullptr != _pointer) {
				if constexpr (std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
					_pointer->~type();
				free(_pointer);
			}
		}

		inline auto operator[](size_type const index) noexcept -> type& {
			return _pointer[index];
		}
		inline void set(type* value) noexcept {
			_pointer = value;
		}
		inline void reset(void) noexcept {
			_pointer = nullptr;
		}
		inline auto get(void) const noexcept -> type* {
			return _pointer;
		}
		template <class type>
		friend inline bool operator==(unique_pointer<type> const& value, nullptr_t) noexcept {
			return value._pointer == nullptr;
		}
	};
}