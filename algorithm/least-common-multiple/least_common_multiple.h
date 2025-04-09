#pragma once
#include <concepts>
#include "../greatest-common-divisor/greatest_common_divisor.h"

namespace library::algorithm {
	template <typename type>
	inline constexpr auto least_common_multiple(type first, type second) noexcept {
		return first* second / greatest_common_divisor(first, second);
	}

	template <typename type_1, typename... type_2>
	inline constexpr auto least_common_multiple(type_1 first, type_2... second) noexcept {
		return least_common_multiple(first, least_common_multiple(second...));
	}
}