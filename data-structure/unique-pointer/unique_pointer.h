#pragma once
#include <malloc.h>
#include <utility>
#include <type_traits>
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace library {
	template<typename type>
	class unique_pointer final {
		type* _pointer;
	public:
		inline constexpr unique_pointer(void) noexcept
			: _pointer(nullptr) {
		};
		inline constexpr unique_pointer(nullptr_t) noexcept
			: _pointer(nullptr) {
		};
		inline explicit unique_pointer(type* const pointer) noexcept
			: _pointer(pointer) {
		}
		template<typename... argument>
		inline explicit unique_pointer(argument&&... arg) noexcept {
			_pointer = library::allocate<type>();
			library::construct<type>(*_pointer, std::forward<argument>(arg)...);
		}
		inline explicit unique_pointer(unique_pointer&) noexcept = delete;
		inline explicit unique_pointer(unique_pointer&& rhs) noexcept
			: _pointer(library::exchange(rhs._pointer, nullptr)) {
		};
		inline auto operator=(unique_pointer const&) noexcept -> unique_pointer & = delete;
		inline auto operator=(unique_pointer&& rhs) noexcept -> unique_pointer& {
			unique_pointer(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~unique_pointer(void) noexcept {
			if (nullptr != _pointer) {
				library::destruct<type>(*_pointer);
				library::deallocate<type>(_pointer);
			}
		}

		inline auto operator*(void) noexcept -> type& {
			return *_pointer;
		}
		inline auto operator->(void) noexcept -> type* {
			return _pointer;
		}
		inline explicit operator bool() const noexcept {
			return nullptr != _pointer;
		}
		inline void swap(unique_pointer& rhs) noexcept {
			library::swap(_pointer, rhs._pointer);
		}
		inline auto get(void) const noexcept -> type* {
			return _pointer;
		}
		inline void set(type* value) noexcept {
			_pointer = value;
		}
		inline void reset(void) noexcept {
			_pointer = nullptr;
		}
		inline friend bool operator==(unique_pointer const& value, nullptr_t) noexcept {
			return value._pointer == nullptr;
		}
	};
	template<typename type>
	class unique_pointer<type[]> final {
		using size_type = unsigned int;
		type* _pointer;
	public:
		inline constexpr unique_pointer(void) noexcept
			: _pointer(nullptr) {
		};
		inline constexpr unique_pointer(nullptr_t) noexcept
			: _pointer(nullptr) {
		};
		inline explicit unique_pointer(type* const pointer) noexcept
			: _pointer(pointer) {
		}
		inline explicit unique_pointer(unique_pointer&) noexcept = delete;
		inline explicit unique_pointer(unique_pointer&& rhs) noexcept
			: _pointer(library::exchange(rhs._pointer, nullptr)) {
		}
		inline auto operator=(unique_pointer const&) noexcept -> unique_pointer & = delete;
		inline auto operator=(unique_pointer&& rhs) noexcept -> unique_pointer& {
			unique_pointer(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~unique_pointer(void) noexcept {
			if (nullptr != _pointer) {
				library::destruct<type>(*_pointer);
				library::deallocate<type>(_pointer);
			}
		}

		inline auto operator[](size_type const index) noexcept -> type& {
			return _pointer[index];
		}
		inline explicit operator bool() const noexcept {
			return nullptr != _pointer;
		}
		inline void swap(unique_pointer& rhs) noexcept {
			library::swap(_pointer, rhs._pointer);
		}
		inline auto get(void) const noexcept -> type* {
			return _pointer;
		}
		inline void set(type* value) noexcept {
			_pointer = value;
		}
		inline void reset(void) noexcept {
			_pointer = nullptr;
		}
		template <class type>
		friend inline bool operator==(unique_pointer<type> const& lhs, nullptr_t) noexcept {
			return nullptr == lhs._pointer;
		}
	};
}