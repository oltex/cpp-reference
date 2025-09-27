#pragma once

namespace detail {
	template <typename _type>
	struct remove_const {
		using type = _type;
	};
	template <typename _type>
	struct remove_const<_type const> {
		using type = _type;
	};

	template <typename _type>
	struct remove_volatile {
		using type = _type;
	};
	template <typename _type>
	struct remove_volatile<volatile _type> {
		using type = _type;
	};

	template <typename _type>
	struct remove_reference {
		using type = _type;
	};
	template <typename _type>
	struct remove_reference<_type&> {
		using type = _type;
	};
	template <typename _type>
	struct remove_reference<_type&&> {
		using type = _type;
	};

	template <typename _type>
	struct remove_pointer {
		using type = _type;
	};
	template <typename _type>
	struct remove_pointer<_type*> {
		using type = _type;
	};
	template <typename _type>
	struct remove_pointer<_type* const> {
		using type = _type;
	};
	template <typename _type>
	struct remove_pointer<_type* volatile> {
		using type = _type;
	};
	template <typename _type>
	struct remove_pointer<_type* const volatile> {
		using type = _type;
	};

	template <class _type>
	struct type_identity {
		using type = _type;
	};
}

namespace library {
	template <typename type>
	using remove_const = typename detail::remove_const<type>::type;
	template <typename type>
	using remove_volatile = typename detail::remove_volatile<type>::type;
	template <typename type>
	using remove_reference = typename detail::remove_reference<type>::type;
	template <typename type>
	using remove_pointer = typename detail::remove_pointer<type>::type;
	template <typename type>
	using remove_cp = typename detail::remove_const<typename detail::remove_pointer<type>::type>::type;
	template <typename type>
	using remove_cv = typename detail::remove_const<typename detail::remove_volatile<type>::type>::type;
	template <typename type>
	using remove_rp = typename detail::remove_reference<typename detail::remove_pointer<type>::type>::type;

	template <class type>
	using type_identity = detail::type_identity<type>::type;

	template <typename, typename>
	inline constexpr bool same_type = false;
	template <typename type>
	inline constexpr bool same_type<type, type> = true;
	template <typename type, typename... rest>
	inline constexpr bool any_of_type = (same_type<type, rest> || ...);
	template <typename type>
	inline constexpr bool void_type = same_type<remove_cv<type>, void>;
	template <typename>
	inline constexpr bool const_type = false;
	template <typename type>
	inline  constexpr bool const_type<type const> = true;
	template <typename>
	inline constexpr bool pointer_type = false;
	template <typename type>
	inline constexpr bool pointer_type<type*> = true;
	template <typename type>
	inline constexpr bool pointer_type<type* const> = true;
	template <typename type>
	inline constexpr bool pointer_type<type* volatile> = true;
	template <typename type>
	inline constexpr bool pointer_type<type* const volatile> = true;
	template <typename>
	inline constexpr bool reference_type = false;
	template <typename type>
	inline constexpr bool reference_type<type&> = true;
	template <typename type>
	inline constexpr bool reference_type<type&&> = true;
	template <typename>
	inline constexpr bool rvalue_reference_type = false;
	template <typename type>
	inline constexpr bool rvalue_reference_type<type&&> = true;
	template <typename type>
	inline constexpr bool integral_type = any_of_type<remove_cv<type>, bool, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;
	template <typename type>
	inline constexpr bool floating_point_type = any_of_type<remove_cv<type>, float, double, long double>;
	template <typename type>
	inline constexpr bool arithmetic_type = integral_type<type> || floating_point_type<type>;
	//template <typename type>
	//inline constexpr bool fundamental_type = arithmetic_type<type> || void_type<type> || is_null_pointer_v<type>;


	template <typename type, typename... argument>
	struct key_exist_set {
		static constexpr bool able = false;
	};
	template <typename type>
	struct key_exist_set<type, type> {
		static constexpr bool able = true;
		static auto execute(type const& value) noexcept -> type const& {
			return value;
		}
	};

	template <typename key_type, typename... argument>
	struct key_exist_map {
		static constexpr bool able = false;
	};
	template <typename key_type, typename type>
	struct key_exist_map<key_type, key_type, type> {
		static constexpr bool able = true;
		static auto execute(key_type const& key, type const&) noexcept -> key_type const& {
			return key;
		}
	};
}