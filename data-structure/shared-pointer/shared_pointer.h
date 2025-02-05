#pragma once
#include <malloc.h>
#include <utility>
#include <type_traits>

namespace data_structure {
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
			: _value(nullptr), _reference(nullptr) {
		}
		inline constexpr shared_pointer(nullptr_t) noexcept
			: _value(nullptr), _reference(nullptr) {
		};
		inline explicit shared_pointer(type* value) noexcept 
			: _value(value) {
			_reference = static_cast<reference*>(malloc(sizeof(reference)));
#pragma warning(suppress: 6011)
			_reference->_use = 1;
			_reference->_weak = 0;
		}
		template<typename... argument>
		inline explicit shared_pointer(argument&&... arg) noexcept {
			_value = static_cast<type*>(malloc(sizeof(type)));
			_reference = static_cast<reference*>(malloc(sizeof(reference)));
			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					::new(reinterpret_cast<void*>(_value)) type(std::forward<argument>(arg)...);
			}
			else if constexpr (1 == sizeof...(arg))
#pragma warning(suppress: 6011)
				* _value = type(std::forward<argument>(arg)...);
			_reference->_use = 1;
			_reference->_weak = 0;
		}
		inline shared_pointer(shared_pointer& rhs) noexcept
			: _value(rhs._value), _reference(rhs._reference) {
			if (nullptr != _reference)
				++_reference->_use;
		};
		inline explicit shared_pointer(shared_pointer&& rhs) noexcept
			: _value(rhs._value), _reference(rhs._reference) {
			rhs._value = nullptr;
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
				if constexpr (std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
					_value->~type();
				free(_value);
				if (0 == _reference->_weak)
					free(_reference);
			}
		}
	public:
		inline auto operator*(void) noexcept -> type& {
			return *_value;
		}
		inline auto operator->(void) noexcept -> type* {
			return _value;
		}
		inline void swap(shared_pointer& rhs) noexcept {
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
		inline auto get(void) const noexcept -> type* {
			return _value;
		}
		template <class type>
		friend inline bool operator==(shared_pointer<type> const& value, nullptr_t) noexcept {
			return value._value == nullptr;
		}
	private:
		type* _value;
		reference* _reference;
	};

	//template <class type>
	//inline bool operator==(shared_pointer<type> const& value, nullptr_t) noexcept {
	//	return value.get() == nullptr;
	//}
}