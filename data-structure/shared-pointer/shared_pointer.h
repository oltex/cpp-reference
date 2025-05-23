#pragma once
#include <malloc.h>
#include <utility>
#include <type_traits>
#include "../../system/memory/memory.h"
#include "../../algorithm/swap/swap.h"

namespace library::data_structure {
	struct reference final {
	private:
		using size_type = unsigned int;
	public:
		size_type _use;
		size_type _weak;
	};

	template<typename type>
	class weak_pointer;
	template<typename type>
	class shared_pointer final {
	private:
		using size_type = unsigned int;
		friend class weak_pointer<type>;
	public:
		inline constexpr explicit shared_pointer(void) noexcept
			: _pointer(nullptr), _reference(nullptr) {
		}
		inline constexpr shared_pointer(nullptr_t) noexcept
			: _pointer(nullptr), _reference(nullptr) {
		};
		inline explicit shared_pointer(type* pointer) noexcept
			: _pointer(pointer) {
			_reference = system::memory::allocate<reference>();
#pragma warning(suppress: 6011)
			_reference->_use = 1;
			_reference->_weak = 0;
		}
		template<typename... argument>
		inline explicit shared_pointer(argument&&... arg) noexcept {
			_pointer = system::memory::allocate<type>();
			_reference = system::memory::allocate<reference>();
			system::memory::construct(*_pointer, std::forward<argument>(arg)...);
			_reference->_use = 1;
			_reference->_weak = 0;
		}
		inline shared_pointer(shared_pointer& rhs) noexcept
			: shared_pointer(static_cast<shared_pointer const&>(rhs)) {
		};
		inline shared_pointer(shared_pointer const& rhs) noexcept
			: _pointer(rhs._pointer), _reference(rhs._reference) {
			if (nullptr != _reference)
				++_reference->_use;
		};
		inline explicit shared_pointer(shared_pointer&& rhs) noexcept
			: _pointer(rhs._pointer), _reference(rhs._reference) {
			rhs._pointer = nullptr;
			rhs._reference = nullptr;
		};
		inline auto operator=(shared_pointer const& rhs) noexcept -> shared_pointer& {
			shared_pointer(rhs).swap(*this);
			return *this;
		}
		inline auto operator=(shared_pointer&& rhs) noexcept -> shared_pointer& {
			shared_pointer(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~shared_pointer(void) noexcept {
			if (nullptr != _reference && 0 == --_reference->_use) {
				system::memory::destruct<type>(*_pointer);
				system::memory::deallocate<type>(_pointer);
				if (0 == _reference->_weak)
					system::memory::deallocate<reference>(_reference);
			}
		}
	public:
		inline auto operator*(void) noexcept -> type& {
			return *_pointer;
		}
		inline auto operator->(void) noexcept -> type* {
			return _pointer;
		}
		inline void swap(shared_pointer& rhs) noexcept {
			algorithm::swap(_pointer, rhs._pointer);
			algorithm::swap(_reference, rhs._reference);
		}
		inline auto use_count(void) const noexcept -> size_type {
			return _reference->_use;
		}
		inline auto get(void) const noexcept -> type* {
			return _pointer;
		}
		template <class type>
		friend inline bool operator==(shared_pointer<type> const& rhs, nullptr_t) noexcept {
			return rhs._pointer == nullptr;
		}
	private:
		type* _pointer;
		reference* _reference;
	};
}