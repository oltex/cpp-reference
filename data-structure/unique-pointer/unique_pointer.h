#pragma once
#include <malloc.h>
#include <utility>
#include <type_traits>
#include "../../system-component/memory/memory.h"

namespace library::data_structure {
	template<typename type>
	class unique_pointer final {
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
			_pointer = system_component::memory::allocate<type>();
			system_component::memory::construct<type>(*_pointer, std::forward<argument>(arg)...);
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
				system_component::memory::destruct<type>(*_pointer);
				system_component::memory::deallocate<type>(_pointer);
			}
		}
	public:
		inline auto operator*(void) noexcept -> type& {
			return *_pointer;
		}
		inline auto operator->(void) noexcept -> type* {
			return _pointer;
		}
	public:
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
	private:
		type* _pointer;
	};


	template<typename type>
	class unique_pointer<type[]> final {
	private:
		using size_type = unsigned int;
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
	public:
		inline auto operator[](size_type const index) noexcept -> type& {
			return _pointer[index];
		}
	public:
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
	private:
		type* _pointer;
	};
}