#pragma once

template <class _Ty, class _Uy>
struct pair final {
	template<typename _Ty, typename... _Arg>
	inline explicit pair(_Ty key, _Arg&&... argument) noexcept
		: _first(key), _second(std::forward<_Arg>(argument)...) {
	}
	_Ty _first;
	_Uy _second;
};