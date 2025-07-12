#pragma once
#include <compare>

namespace library::predicate {
	template<typename type>
	inline constexpr auto less(type const& source, type const& destination) noexcept {
		return source < destination;
	}
	template<typename type>
	inline constexpr auto greater(type const& source, type const& destination) noexcept {
		return destination < source;
	}

	template<typename type>
	inline constexpr auto ordering(type const& source, type const& destination) noexcept {
		return source <=> destination;
	}
}
