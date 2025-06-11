#pragma once
#include <utility>
#include <type_traits>

namespace library::data_structure {
	template<typename... type>
	class tuple;
	template<>
	class tuple<> {
	};
	template<typename type, typename... rest>
	class tuple<type, rest...> : private tuple<rest...> {
		friend class element;
	public:
		template <size_t index, class tuple>
		struct element;
		template <class _this, class... rest>
		struct element<0, tuple<_this, rest...>> {
			using type = _this;
			using _Ttype = tuple<_this, rest...>;
		};
		template <size_t index, class _this, class... rest>
		struct element<index, tuple<_this, rest...>> : element<index - 1, tuple<rest...>> {
		};

		inline explicit constexpr tuple(void) noexcept = default;
		template<typename type_argument, typename... rest_argument>
		inline explicit constexpr tuple(type_argument&& type_arg, rest_argument&&... rest_arg) noexcept
			: tuple<rest...>(std::forward<rest_argument>(rest_arg)...), _value(std::forward<type_argument>(type_arg)) {
		}
		inline constexpr tuple(tuple&) noexcept = default;
		inline explicit constexpr tuple(tuple&&) noexcept = default;
		inline constexpr auto operator=(tuple& rhs) noexcept -> tuple& {
			_value = rhs._value;
			static_cast<tuple<rest...>&>(*this) = static_cast<tuple<rest...>&>(rhs);
			//get_rest() = rhs.get_rest();
			return *this;
		}
		inline constexpr auto operator=(tuple&& rhs) noexcept -> tuple& {
			_value = std::move(rhs._value);
			static_cast<tuple<rest...>&>(*this) = std::move(static_cast<tuple<rest...>&>(rhs));
			//get_rest() = std::move(rhs.get_rest());
			return *this;
		}
		inline ~tuple(void) noexcept = default;

		template <size_t index>
		inline auto get(void) noexcept -> element<index, tuple>::type& {
			return reinterpret_cast<element<index, tuple>::_Ttype&>(*this)._value;
		}

		type _value;
	};
}

template <class... _Types>
struct std::tuple_size<library::data_structure::tuple<_Types...>> : integral_constant<size_t, sizeof...(_Types)> {
};
template <size_t _Index>
struct _MSVC_KNOWN_SEMANTICS std::tuple_element<_Index, library::data_structure::tuple<>> {
	static_assert(_Always_false<integral_constant<size_t, _Index>>, "tuple index out of bounds");
};
template <class _This, class... _Rest>
struct _MSVC_KNOWN_SEMANTICS std::tuple_element<0, library::data_structure::tuple<_This, _Rest...>> {
	using type = _This;
	using _Ttype = tuple<_This, _Rest...>;
};
template <size_t _Index, class _This, class... _Rest>
struct _MSVC_KNOWN_SEMANTICS std::tuple_element<_Index, library::data_structure::tuple<_This, _Rest...>>
	: tuple_element<_Index - 1, tuple<_Rest...>> {
};