#pragma once
#include "../pair.h"
#include "../tuple.h"
#include "my_class.h"

#include <tuple>

namespace example {
	inline void pair(void) noexcept {
		struct test {
		};
		library::pair<int, test> pair2;
		std::pair<int, int>(std::piecewise_construct, std::forward_as_tuple(10), std::forward_as_tuple(10));
		library::pair<int, int> pair(library::_piecewise_construct, library::forward_as_tuple(10), library::forward_as_tuple(10));

		//pair._first = 0;
		//pair._second = 1;
	}
}