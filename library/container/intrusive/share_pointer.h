#pragma once
#include "../function.h"
#include "../../system/interlock.h"
#include <malloc.h>
#include <utility>
#include <type_traits>

namespace library::intrusive {
	template<size_t index>
	class share_pointer_hook {
		using size_type = unsigned int;
		template<typename type, size_t>
		friend class share_pointer;
		size_type _use;
	public:
		inline explicit share_pointer_hook(void) noexcept = default;
		inline explicit share_pointer_hook(share_pointer_hook const&) noexcept = default;
		inline explicit share_pointer_hook(share_pointer_hook&&) noexcept = default;
		inline auto operator=(share_pointer_hook const&) noexcept -> share_pointer_hook & = default;
		inline auto operator=(share_pointer_hook&&) noexcept -> share_pointer_hook & = default;
		inline ~share_pointer_hook(void) noexcept = default;
	};

	template<typename type, size_t index>
	class share_pointer final {
		using size_type = unsigned int;
		using hook = share_pointer_hook<index>;
		static_assert(std::is_base_of<hook, type>::value);
		hook* _pointer;
	public:
		inline constexpr explicit share_pointer(void) noexcept
			: _pointer(nullptr) {
		}
		inline constexpr share_pointer(nullptr_t) noexcept
			: _pointer(nullptr) {
		};
		inline explicit share_pointer(type* value) noexcept {
			_pointer = static_cast<hook*>(value);
			library::interlock_exchange(_pointer->_use, 1);
		}
		inline share_pointer(share_pointer const& rhs) noexcept
			: _pointer(rhs._pointer) {
			if (nullptr != _pointer)
				library::interlock_increment(_pointer->_use);
		};
		inline explicit share_pointer(share_pointer&& rhs) noexcept
			: _pointer(library::exchange(rhs._pointer, nullptr)) {
		};
		inline auto operator=(share_pointer const& rhs) noexcept -> share_pointer& {
			share_pointer(rhs).swap(*this);
			return *this;
		}
		inline auto operator=(share_pointer&& rhs) noexcept -> share_pointer& {
			share_pointer(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~share_pointer(void) noexcept {
			if (nullptr != _pointer && 0 == library::interlock_decrement(_pointer->_use))
				static_cast<type*>(_pointer)->template destructor<index>();
		}

		inline auto operator*(void) const noexcept -> type& {
			return static_cast<type&>(*_pointer);
		}
		inline auto operator->(void) const noexcept -> type* const {
			return static_cast<type*>(_pointer);
		}
		inline explicit operator bool() const noexcept {
			return nullptr != _pointer;
		}
		inline void swap(share_pointer& rhs) noexcept {
			library::swap(_pointer, rhs._pointer);
		}
		inline auto use_count(void) const noexcept -> size_type {
			return _pointer._use;
		}
		inline auto get(void) const noexcept -> type* {
			return static_cast<type*>(_pointer);
		}
		inline void set(type* value) noexcept {
			_pointer = static_cast<hook*>(value);
		}
		inline void reset(void) noexcept {
			_pointer = nullptr;
		}
		friend inline bool operator==(share_pointer const& lhs, nullptr_t) noexcept {
			return nullptr == lhs._pointer;
		}
	};
}