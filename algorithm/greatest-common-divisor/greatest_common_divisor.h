#pragma once
#include <concepts>

namespace library::algorithm {
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
}