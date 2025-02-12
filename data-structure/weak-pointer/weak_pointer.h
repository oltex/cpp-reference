#pragma once
#include "../shared-pointer/shared_pointer.h"

namespace data_structure {
	template<typename type>
	class weak_pointer final {
	private:
		using size_type = unsigned int;
	public:
		inline constexpr explicit weak_pointer(void) noexcept
			: _value(nullptr), _reference(nullptr) {
		}
		inline weak_pointer(shared_pointer<type> const& shared_ptr) noexcept
			: _value(shared_ptr._value), _reference(shared_ptr._reference) {
			if (nullptr != _reference)
				++_reference->_weak;
		}
		inline explicit weak_pointer(weak_pointer const& rhs) noexcept
			: _value(rhs._value), _reference(rhs._reference) {
			if (nullptr != _reference)
				++_reference->_weak;
		};
		inline explicit weak_pointer(weak_pointer&& rhs) noexcept
			: _value(rhs._value), _reference(rhs._reference) {
			rhs._value = nullptr;
			rhs._reference = nullptr;
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
			if (nullptr != _reference &&
				0 == --_reference->_weak && 0 == _reference->_use)
				free(_reference);
		}
	public:
		inline auto operator*(void) noexcept -> type& {
			return *_value;
		}
		inline auto operator->(void) noexcept -> type* {
			return _value;
		}
	public:
		inline void swap(weak_pointer& rhs) noexcept {
			{
				auto temp = _value;
				_value = rhs._value;
				rhs._value = temp;
			}
			{
				auto temp = _reference;
				_reference = rhs._reference;
				rhs._reference = temp;
			}
		}
		inline auto use_count(void) const noexcept -> size_type {
			return _reference->_use;
		}
		inline bool expired(void) const noexcept {
			return 0 == _reference->_use;
		}
		inline auto get(void) const noexcept -> type* {
			return _value;
		}
	private:
		type* _value;
		reference* _reference;
	};
}