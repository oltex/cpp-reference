#pragma once
#include <utility>
#include <type_traits>

namespace data_structure {
	template <size_t index, class tuple>
	struct tuple_element;

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
		template <size_t _Index, class... _Types>
		friend constexpr tuple_element<_Index, tuple<_Types...>>::type& get(tuple<_Types...>& _Tuple) noexcept;
	public:
		type _value;
	};

	template <class _This, class... _Rest>
	struct tuple_element<0, tuple<_This, _Rest...>> {
		using type = _This;
		using _Ttype = tuple<_This, _Rest...>;
	};
	template <size_t index, class _This, class... _Rest>
	struct tuple_element<index, tuple<_This, _Rest...>> : tuple_element<index - 1, tuple<_Rest...>> {
	};

	template <size_t index, class... _Types>
	inline static constexpr tuple_element<index, tuple<_Types...>>::type& get(tuple<_Types...>& _Tuple) noexcept {
		return static_cast<tuple_element<index, tuple<_Types...>>::_Ttype&>(_Tuple)._value;
	}
}