#pragma once
#define _WINSOCKAPI_
#include <utility>
#include <type_traits>
#include <cmath>
#include <Windows.h>
#include "template.h"

namespace library {
	template<typename type, typename size_type = unsigned long long>
	inline constexpr auto fnv_hash(type const& key) noexcept -> size_type {
		constexpr size_type _fnv_offset_basis = sizeof(size_type) == 4 ? 2166136261U : 14695981039346656037ULL;
		constexpr size_type _fnv_prime = sizeof(size_type) == 4 ? 16777619U : 1099511628211ULL;

		size_type value = _fnv_offset_basis;
		unsigned char const* const byte = reinterpret_cast<unsigned char const*>(&key);
		for (size_type index = 0; index < sizeof(type); ++index) {
			value ^= static_cast<size_type>(byte[index]);
			value *= _fnv_prime;
		}
		return value;
	}

	template<typename type>
	inline constexpr auto less(type const& left, type const& right) noexcept {
		return left < right;
	}
	template<typename type = void>
	class lesser final {
		inline constexpr auto operator()(type const& left, type const& right) noexcept {
			return right < left;
		}
	};
	template<>
	class lesser<void> final {
		template <typename type>
		inline constexpr auto operator()(type const& left, type const& right) noexcept {
			return right < left;
		}
	};
	template<typename type>
	inline constexpr auto great(type const& left, type const& right) noexcept {
		return right < left;
	}
	template<typename type>
	inline constexpr auto ordering(type const& left, type const& right) noexcept {
		return left <=> right;
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
	inline constexpr auto maximum(type const& first, argument const&... second) noexcept -> type const& {
		return maximum(first, maximum(second...));
	}
	template<typename type>
	inline constexpr auto minimum(type const& first, type const& second) noexcept -> type const& {
		if (first > second)
			return second;
		else
			return first;
	}
	template<typename type, typename... argument>
	inline constexpr auto minimum(type const& first, argument const&... second) noexcept -> type const& {
		return maximum(first, maximum(second...));
	}

	template<typename type>
	inline constexpr auto absolute(type const value) noexcept {
		if constexpr (std::floating_point<type>)
			return std::signbit(value) ? -value : value;
		else
			return value < type(0) ? -value : value;
	}

	template<typename type>
	inline constexpr auto bit_ceil(type const number) noexcept -> type {
		type result = 1;
		while (result < number)
			result <<= 1;
		return result;
	}
	template<typename type>
	inline constexpr auto bit_floor(type const number) noexcept -> type {
		type result = 1;
		while ((result << 1) <= number)
			result <<= 1;
		return result;
	}
	template<typename type>
		requires library::any_of_type<type, unsigned int, int, unsigned long, long, unsigned long long, long long>
	inline constexpr auto bit_scan_forward(type const mask) noexcept -> unsigned long {
		unsigned long index;
		if constexpr (4 == sizeof(type))
			::_BitScanForward(&index, mask);
		else
			::_BitScanForward64(&index, mask);
		return index;
	}
	template<typename type>
		requires library::any_of_type<type, unsigned int, int, unsigned long, long, unsigned long long, long long>
	inline constexpr auto bit_scan_reverse(type const mask) noexcept -> unsigned long {
		unsigned long index;
		if constexpr (4 == sizeof(type))
			::_BitScanReverse(&index, mask);
		else
			::_BitScanReverse64(&index, mask);
		return index;
	}
	template<typename type>
		requires library::any_of_type<type, unsigned int, int, unsigned long, long, unsigned long long, long long>
	inline constexpr auto bit_mask_forward(type const mask) noexcept -> unsigned long {
		return 1 << bit_scan_forward(mask);
	}
	template<typename type>
		requires library::any_of_type<type, unsigned int, int, unsigned long, long, unsigned long long, long long>
	inline constexpr auto bit_mask_reverse(type const mask) noexcept -> unsigned long {
		return 1 << bit_scan_reverse(mask);
	}

	template <typename type>
	inline constexpr auto greatest_common_divisor(type first, type second) noexcept {
		while (second != 0) {
			type temp = second;
			second = first % second;
			first = temp;
		}
		return first;
	}
	template <typename type_1, typename... type_2>
	inline constexpr auto greatest_common_divisor(type_1 first, type_2... second) noexcept {
		return greatest_common_divisor(first, greatest_common_divisor(second...));
	}
	template <typename type>
	inline constexpr auto least_common_multiple(type first, type second) noexcept {
		return first * second / greatest_common_divisor(first, second);
	}
	template <typename type_1, typename... type_2>
	inline constexpr auto least_common_multiple(type_1 first, type_2... second) noexcept {
		return least_common_multiple(first, least_common_multiple(second...));
	}

	template <typename iterator, class _Diff>
	inline constexpr void advance(iterator& iter, _Diff offset) noexcept {
		if constexpr (std::_Is_ranges_random_iter_v<iterator>)
			iter += offset;
		else {
			for (; offset < 0; ++offset)
				--iter;
			for (; 0 < offset; --offset)
				++iter;
		}
	}
	template <typename iterator>
	inline constexpr auto next(iterator iter) noexcept -> iterator {
		library::advance(iter, 1);
		return iter;
	}
	template <typename iterator>
	inline constexpr auto prev(iterator iter) noexcept -> iterator {
		library::advance(iter, -1);
		return iter;
	}

	inline auto string_length(char const* const character) noexcept -> size_t {
		return ::strlen(character);
	}
	inline auto string_length(wchar_t const* const wide_character) noexcept -> size_t {
		return ::wcslen(wide_character);
	}
}