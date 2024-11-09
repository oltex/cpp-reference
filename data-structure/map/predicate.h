#pragma once

template<typename type>
struct less final {
	char operator()(type const& sour, type const& dest) const noexcept {
		if (sour == dest)
			return 0;
		return sour < dest ? 1 : -1;
	}
};

template<typename type>
struct greater final {
	char operator()(type const& sour, type const& dest) {
		if (sour == dest)
			return 0;
		return sour > dest ? 1 : -1;
	}
};