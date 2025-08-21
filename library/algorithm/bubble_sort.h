#pragma once
#include "../function.h"

namespace library {
	template<typename iterator, auto predicate = library::less<decltype(*std::declval<iterator>())>>
	inline void bubble_sort(iterator first, iterator last) noexcept {
		if (first == last)
			return;
		auto end = last;
		while (end != first) {
			bool swap = false;
			auto new_end = first;

			for (auto iter = first, next = library::next(first); next != end; ++iter, ++next) {
				if (predicate(*next, *iter)) {
					library::swap(*iter, *next);
					swap = true;
					new_end = next;
				}
			}
			end = new_end;
			if (!swap)
				break;
		}
	}
}