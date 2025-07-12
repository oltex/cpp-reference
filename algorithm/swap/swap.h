#pragma once
#include <utility>

namespace library {
	template<typename type>
	inline constexpr auto less(type const& source, type const& destination) noexcept {
		return source < destination;
	}
	template<typename type>
	inline constexpr auto greater(type const& source, type const& destination) noexcept {
		return destination < source;
	}
	template<typename type>
	inline constexpr auto ordering(type const& source, type const& destination) noexcept {
		return source <=> destination;
	}

	template <class type, class other = type>
	inline constexpr auto exchange(type& value, other&& new_value) noexcept -> type {
		type old_value = static_cast<type&&>(value);
		value = static_cast<other&&>(new_value);
		return old_value;
	}
	template<typename type>
	inline void swap(type& left, type& right) noexcept {
		type temp = std::move(left);
		left = std::move(right);
		right = std::move(temp);
	}

	template<typename type>
	inline constexpr auto maximum(type const& first, type const& second) noexcept -> type const& {
		if (first < second)
			return second;
		else
			return first;
	}
	template<typename type, typename... argument>
	inline constexpr auto maximum(type const& value, argument const&... arg) noexcept -> type const& {
		return maximum(value, maximum(arg...));
	}
	template<typename type>
	inline constexpr auto minimum(type const& first, type const& second) noexcept -> type const& {
		if (first > second)
			return second;
		else
			return first;
	}
	template<typename type, typename... argument>
	inline constexpr auto minimum(type const& value, argument const&... arg) noexcept -> type const& {
		return maximum(value, maximum(arg...));
	}
}

//_CONSTEXPR20 void swap(_Ty& _Left, _Ty& _Right) noexcept(
//    is_nothrow_move_constructible_v<_Ty>&& is_nothrow_move_assignable_v<_Ty>) {
//    _Ty _Tmp = _STD move(_Left);
//    _Left = _STD move(_Right);
//    _Right = _STD move(_Tmp);
//}
//
//_EXPORT_STD template <class _Ty, size_t _Size, enable_if_t<_Is_swappable<_Ty>::value, int> /* = 0 */>
//_CONSTEXPR20 void swap(_Ty(&_Left)[_Size], _Ty(&_Right)[_Size]) noexcept(_Is_nothrow_swappable<_Ty>::value) {
//    if (&_Left != &_Right) {
//        _Ty* _First1 = _Left;
//        _Ty* _Last1 = _First1 + _Size;
//        _Ty* _First2 = _Right;
//        for (; _First1 != _Last1; ++_First1, ++_First2) {
//            swap(*_First1, *_First2); // intentional ADL
//        }
//    }
//}