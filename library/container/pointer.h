#pragma once
#include "../memory.h"
#include "../function.h"
#include <malloc.h>
#include <utility>
#include <type_traits>

namespace library {
	template<typename type>
	class unique_pointer final {
		template <typename other>
		friend class unique_pointer;
		type* _pointer;
	public:
		inline constexpr unique_pointer(void) noexcept
			: _pointer(nullptr) {
		};
		inline constexpr unique_pointer(nullptr_t) noexcept
			: _pointer(nullptr) {
		};
		template<typename other>
			requires std::is_convertible_v<other*, type*>
		inline explicit unique_pointer(other* const pointer) noexcept
			: _pointer(pointer) {
		}
		template<typename... argument>
		inline explicit unique_pointer(argument&&... arg) noexcept {
			_pointer = library::allocate<type>();
			library::construct<type>(*_pointer, std::forward<argument>(arg)...);
		}
		inline explicit unique_pointer(unique_pointer&) noexcept = delete;
		inline unique_pointer(unique_pointer&& rhs) noexcept
			: _pointer(library::exchange(rhs._pointer, nullptr)) {
		};
		template<typename other>
			requires std::is_convertible_v<other*, type*>
		inline unique_pointer(unique_pointer<other>&& rhs) noexcept
			: _pointer(static_cast<type*>(library::exchange(rhs._pointer, nullptr))) {
		};
		inline auto operator=(unique_pointer const&) noexcept -> unique_pointer & = delete;
		inline auto operator=(unique_pointer&& rhs) noexcept -> unique_pointer& {
			unique_pointer(std::move(rhs)).swap(*this);
			return *this;
		};
		template<typename other>
			requires std::is_convertible_v<other*, type*>
		inline auto operator=(unique_pointer<other>&& rhs) noexcept -> unique_pointer& {
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
		friend inline bool operator==(unique_pointer const& lhs, nullptr_t) noexcept {
			return nullptr == lhs._pointer;
		}
	};
	struct reference final {
	private:
		using size_type = unsigned int;
	public:
		size_type _use;
		size_type _weak;
	};
	template<typename type>
	inline void share_pointer_deleter(type* pointer) noexcept {
		library::destruct<type>(*pointer);
		library::deallocate<type>(pointer);
	}
	template<typename type, auto deleter = share_pointer_deleter<type>>
	class share_pointer final {
		template <typename other, auto>
		friend class share_pointer;
		template<typename type>
		friend class weak_pointer;
		using size_type = unsigned int;
		type* _pointer;
		reference* _reference;
	public:
		inline constexpr explicit share_pointer(void) noexcept
			: _pointer(nullptr), _reference(nullptr) {
		}
		inline share_pointer(share_pointer& rhs) noexcept
			: share_pointer(static_cast<share_pointer const&>(rhs)) {
		};
		inline share_pointer(share_pointer const& rhs) noexcept
			: _pointer(rhs._pointer), _reference(rhs._reference) {
			if (nullptr != _reference)
				++_reference->_use;
		};
		inline share_pointer(share_pointer&& rhs) noexcept
			: _pointer(library::exchange(rhs._pointer, nullptr)), _reference(library::exchange(rhs._reference, nullptr)) {
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
			if (nullptr != _reference && 0 == --_reference->_use) {
				deleter(_pointer);
				if (0 == _reference->_weak)
					library::deallocate<reference>(_reference);
			}
		}
		inline constexpr share_pointer(nullptr_t) noexcept
			: _pointer(nullptr), _reference(nullptr) {
		};
		template<typename other>
			//requires std::is_convertible_v<other*, type*>
		inline explicit share_pointer(other* const pointer) noexcept
			: _pointer(static_cast<type*>(pointer)), _reference(library::allocate<reference>()) {
#pragma warning(suppress: 6011)
			_reference->_use = 1;
			_reference->_weak = 0;
		}
		template<typename... argument>
		inline explicit share_pointer(argument&&... arg) noexcept
			: _pointer(library::allocate<type>()), _reference(library::allocate<reference>()) {
			library::construct(*_pointer, std::forward<argument>(arg)...);
			_reference->_use = 1;
			_reference->_weak = 0;
		}
		template<typename other, auto other_deleter>
			//requires std::is_convertible_v<other*, type*>
		inline share_pointer(share_pointer<other, other_deleter>& rhs) noexcept
			: share_pointer(static_cast<share_pointer<other, other_deleter> const&>(rhs)) {
		};
		template<typename other, auto other_deleter>
			//requires std::is_convertible_v<other*, type*>
		inline share_pointer(share_pointer<other, other_deleter> const& rhs) noexcept
			: _pointer(static_cast<type*>(rhs._pointer)), _reference(rhs._reference) {
			if (nullptr != _reference)
				++_reference->_use;
		};
		template<typename other, auto other_deleter>
			//requires std::is_convertible_v<other*, type*>
		inline share_pointer(share_pointer<other, other_deleter>&& rhs) noexcept
			: _pointer(static_cast<type*>(library::exchange(rhs._pointer, nullptr))), _reference(library::exchange(rhs._reference, nullptr)) {
		};
		template<typename other, auto other_deleter>
			//requires std::is_convertible_v<other*, type*>
		inline auto operator=(share_pointer<other, other_deleter> const& rhs) noexcept -> share_pointer& {
			share_pointer(rhs).swap(*this);
			return *this;
		}
		template<typename other, auto other_deleter>
			//requires std::is_convertible_v<other*, type*>
		inline auto operator=(share_pointer<other, other_deleter>&& rhs) noexcept -> share_pointer& {
			share_pointer(std::move(rhs)).swap(*this);
			return *this;
		};

		inline auto operator*(void) const noexcept -> type& {
			return *_pointer;
		}
		inline auto operator->(void) const noexcept -> type* const {
			return _pointer;
		}
		inline explicit operator bool() const noexcept {
			return nullptr != _pointer;
		}
		inline void swap(share_pointer& rhs) noexcept {
			library::swap(_pointer, rhs._pointer);
			library::swap(_reference, rhs._reference);
		}
		inline auto use_count(void) const noexcept -> size_type {
			return _reference->_use;
		}
		inline auto get(void) const noexcept -> type* {
			return _pointer;
		}
		inline friend bool operator==(share_pointer const& lhs, nullptr_t) noexcept {
			return nullptr == lhs._pointer;
		}
	};
	template<typename type>
	class weak_pointer final {
		using size_type = unsigned int;
		type* _pointer;
		reference* _reference;
	public:
		inline constexpr explicit weak_pointer(void) noexcept
			: _pointer(nullptr), _reference(nullptr) {
		}
		inline weak_pointer(weak_pointer const& rhs) noexcept
			: _pointer(rhs._pointer), _reference(rhs._reference) {
			if (nullptr != _reference)
				++_reference->_weak;
		};
		inline weak_pointer(weak_pointer&& rhs) noexcept
			: _pointer(library::exchange(rhs._pointer, nullptr)), _reference(library::exchange(rhs._reference, nullptr)) {
		};
		inline auto operator=(weak_pointer const& rhs) noexcept -> weak_pointer& {
			weak_pointer(rhs).swap(*this);
			return *this;
		}
		inline auto operator=(weak_pointer&& rhs) noexcept -> weak_pointer& {
			weak_pointer(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~weak_pointer(void) noexcept {
			if (nullptr != _reference && 0 == --_reference->_weak && 0 == _reference->_use)
				library::deallocate<reference>(_reference);
		}
		inline constexpr explicit weak_pointer(nullptr_t) noexcept
			: _pointer(nullptr), _reference(nullptr) {
		}
		template<typename other, auto other_deleter>
			//requires std::is_convertible_v<other*, type*>
		inline weak_pointer(share_pointer<other, other_deleter> const& shared_ptr) noexcept
			: _pointer(shared_ptr._pointer), _reference(shared_ptr._reference) {
			if (nullptr != _reference)
				++_reference->_weak;
		}
		template<typename other>
			//requires std::is_convertible_v<other*, type*>
		inline weak_pointer(weak_pointer<other> const& rhs) noexcept
			: _pointer(static_cast<type*>(rhs._pointer)), _reference(rhs._reference) {
			if (nullptr != _reference)
				++_reference->_weak;
		};
		template<typename other>
			//requires std::is_convertible_v<other*, type*>
		inline weak_pointer(weak_pointer<other>&& rhs) noexcept
			: _pointer(static_cast<type*>(library::exchange(rhs._pointer, nullptr))), _reference(library::exchange(rhs._reference, nullptr)) {
		};
		template<typename other>
			//requires std::is_convertible_v<other*, type*>
		inline auto operator=(weak_pointer<other> const& rhs) noexcept -> weak_pointer& {
			weak_pointer(rhs).swap(*this);
			return *this;
		}
		template<typename other>
			requires std::is_convertible_v<other*, type*>
		inline auto operator=(weak_pointer<other>&& rhs) noexcept -> weak_pointer& {
			weak_pointer(std::move(rhs)).swap(*this);
			return *this;
		};

		inline auto operator*(void) const noexcept -> type& {
			return *_pointer;
		}
		inline auto operator->(void) const noexcept -> type* const {
			return _pointer;
		}
		inline explicit operator bool() const noexcept {
			return nullptr != _pointer;
		}
		inline void swap(weak_pointer& rhs) noexcept {
			library::swap(_pointer, rhs._pointer);
			library::swap(_reference, rhs._reference);
		}
		inline auto use_count(void) const noexcept -> size_type {
			return _reference->_use;
		}
		inline bool expired(void) const noexcept {
			return 0 == _reference->_use;
		}
		inline auto get(void) const noexcept -> type* {
			return _pointer;
		}
	};
}