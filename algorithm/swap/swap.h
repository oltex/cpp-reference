#pragma once
#include <utility>

namespace library::algorithm {
	template<typename type>
	inline void swap(type& left, type& right) noexcept {
		type temp = std::move(left);
		left = std::move(right);
		right = std::move(temp);
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