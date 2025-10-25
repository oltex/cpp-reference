#pragma once
#include "../span.h"
#include <span>

namespace example {
	int t[10] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	inline void span(void) noexcept {
		std::span<int> std_span;
		library::span<int, 10> span(t);
		int k = 10;
	}
}