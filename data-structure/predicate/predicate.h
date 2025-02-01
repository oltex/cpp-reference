#pragma once
#include <compare>

template<typename type>
inline static auto less(type const& source, type const& destination) noexcept {
	return source <=> destination;
}
template<typename type>
inline static auto greater(type const& source, type const& destination) noexcept {
	return destination <=> source;
}


//template<typename type>
//struct less final {
//	inline auto operator()(type const& source, type const& destination) const noexcept {
//		return source <=> destination;
//	}
//};
//
//template<typename type>
//struct greater final {
//	inline auto operator()(type const& source, type const& destination) const noexcept {
//		return destination <=> source;
//	}
//};