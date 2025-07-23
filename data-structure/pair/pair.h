#pragma once
#include <utility>

namespace library {
	struct piecewise_construct_t {
		inline explicit piecewise_construct_t(void) noexcept = default;
	};
	constexpr piecewise_construct_t piecewise_construct{};

	template<typename... type>
	class tuple;
	template <class type_1, class type_2>
	struct pair final {
		inline explicit pair(void) noexcept = default;

		//template <class... type_1, class... type_2>
		//inline explicit pair(piecewise_construct_t, tuple<type_1...> _Val1, tuple<_Types2...> _Val2) noexcept
		//	: pair(_Val1, _Val2, index_sequence_for<type_1...>{}, index_sequence_for<type_2...>{}) {
		//}

		//template <class _Tuple1, class _Tuple2, size_t... _Indices1, size_t... _Indices2>
		//constexpr pair(_Tuple1& _Val1, _Tuple2& _Val2, std::index_sequence<_Indices1...>, std::index_sequence<_Indices2...>) {
		//	_Val1.move<0>();
		//}
		template <class... type_1, class... type_2>
		inline explicit pair(piecewise_construct_t, tuple<type_1...> _Val1, tuple<type_2...> _Val2) noexcept
			/*: pair(_Val1, _Val2, std::index_sequence_for<type_1...>{}, std::index_sequence_for<type_2...>{})*/ {
			_Val1.move<0>();
		}

		type_1 _first;
		type_2 _second;
	};
}