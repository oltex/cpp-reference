#pragma once
#include <compare>

template<typename type>
struct less final {
	inline auto operator()(type const& source, type const& destination) const noexcept -> std::strong_ordering {
		return source <=> destination;
	}
};

template<typename type>
struct greater final {
	inline auto operator()(type const& source, type const& destination) const noexcept {
		return destination <=> source;
	}
};