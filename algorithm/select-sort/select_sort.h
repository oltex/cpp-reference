#pragma once
#include "../../function/function.h"

namespace library {
	template<typename iterator, auto predicate = library::less<>>
	inline void select_sort(iterator first, iterator last) {
		for (auto iter = first; iter != last; ++iter) {
			auto minimum = iter;
			for (auto j = library::next(iter); j != last; ++j)
				if (predicate(*j, *minimum))
					minimum = j;
			library::swap(*iter, *minimum);
		}
	}
}