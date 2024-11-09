#pragma once
#include <utility>
#include <type_traits>

namespace data_structure {
	template<typename... type>
	class tuple;

	template<>
	class tuple<> {
	};

	template<typename type, typename... rest>
	class tuple<type, rest...> : private tuple<rest...> {
	public:
		template<typename type_argument, typename... rest_argument>
		inline explicit constexpr tuple(type_argument&& type_arg, rest_argument&&... rest_arg) noexcept
			: tuple<rest...>(std::forward<rest_argument>(rest_arg)...), _value(std::forward<type_argument>(type_arg)) {
		}
		inline explicit constexpr tuple(tuple&) noexcept = default;
		inline explicit constexpr tuple(tuple&&) noexcept = default;
		inline constexpr auto operator=(tuple& rhs) noexcept -> tuple& {
			_value = rhs._value;
			get_rest() = rhs.get_rest();
			return *this;
		}
		inline constexpr auto operator=(tuple&& rhs) noexcept -> tuple& {
			_value = std::move(rhs._value);
			get_rest() = std::move(rhs.get_rest());
			return *this;
		}
		inline ~tuple(void) noexcept = default;
	public:
		inline auto get_rest(void) noexcept -> tuple<rest...>& {
			return *this;
		}
	public:
		type _value;
	};


	template <size_t index, typename... type>
	constexpr tuple_element_t<index, tuple<type...>>& get(tuple<type...>& _Tuple) noexcept {
		using _Ttype = typename tuple_element<_Index, tuple<_Types...>>::_Ttype;
		return static_cast<_Ttype&>(_Tuple)._Myfirst._Val;
	}
}