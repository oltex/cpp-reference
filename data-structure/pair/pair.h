#pragma once
#include <utility>

namespace library {
	struct piecewise_construct_t {
		inline explicit piecewise_construct_t(void) noexcept = default;
	};
	constexpr piecewise_construct_t piecewise_construct{};

	template<typename... type>
	class tuple;
	template <typename type_1, typename type_2>
	struct pair final {
		inline explicit constexpr pair(void) noexcept = default;
		template <typename type_1, typename type_2>
		inline explicit constexpr pair(type_1 value_1, type_2 value_2) noexcept
			: _first(value_1), _second(value_2) {
		}
		template <class... type_1, class... type_2>
		inline explicit constexpr pair(piecewise_construct_t, tuple<type_1...> value_1, tuple<type_2...> value_2) noexcept
			: pair(value_1, value_2, std::index_sequence_for<type_1...>{}, std::index_sequence_for<type_2...>{}) {
		}
		template <class tuple_1, class tuple_2, size_t... index_1, size_t... index_2>
		inline explicit constexpr pair(tuple_1& value_1, tuple_2& value_2, std::index_sequence<index_1...>, std::index_sequence<index_2...>)
			: _first(value_1.template move<index_1>()...), _second(value_2.template move<index_2>()...) {
		}

		type_1 _first;
		type_2 _second;
	};
}

//_EXPORT_STD /* TRANSITION, VSO-1538698 */ template <size_t _Index, class... _Types>
//_NODISCARD constexpr auto&& _Tuple_get(tuple<_Types...>&& _Tuple) noexcept {
//	// used by pair's piecewise constructor
//	using _Ty = tuple_element_t<_Index, tuple<_Types...>>;
//	using _Ttype = typename tuple_element<_Index, tuple<_Types...>>::_Ttype;
//	return static_cast<_Ty&&>(static_cast<_Ttype&>(_Tuple)._Myfirst._Val);
//}