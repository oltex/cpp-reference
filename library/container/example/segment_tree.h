#pragma once
#include "../segment_tree.h"

namespace example {
	inline void segment_tree(void) noexcept {
		library::segment_tree<int> segment_tree{ 1, 4, -3 ,7,6,-2,0 };
		//segment_tree.update(2, 6);
		auto result = segment_tree.query(0, 2);
		//auto result2 = segment_tree.query2(0, 2);
	}
}