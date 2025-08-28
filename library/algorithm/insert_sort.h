#pragma once
#include "../function.h"

namespace library {
	template<typename iterator, auto predicate = library::less<decltype(*std::declval<iterator>())>>
	inline void select_sort(iterator first, iterator last) noexcept {
	}
}