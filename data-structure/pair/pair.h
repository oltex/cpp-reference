#pragma once

namespace library {
	template <class type_1, class type_2>
	struct pair final {
		//template <class _Tuple1, class _Tuple2, size_t... _Indices1, size_t... _Indices2>
		//constexpr pair(_Tuple1& _Val1, _Tuple2& _Val2, index_sequence<_Indices1...>, index_sequence<_Indices2...>)
		//	: first(_Tuple_get<_Indices1>(_STD move(_Val1))...), second(_Tuple_get<_Indices2>(_STD move(_Val2))...) {}

		//template <class... type_1, class... type_2>
		//inline explicit pair(piecewise_construct_t, tuple<type_1...> _Val1, tuple<_Types2...> _Val2) noexcept
		//	: pair(_Val1, _Val2, index_sequence_for<type_1...>{}, index_sequence_for<type_2...>{}) {
		//}

		type_1 _first;
		type_2 _second;
	};
}