#pragma once
#include "../bit_grid.h"

namespace example {
	inline void bit_grid(void) noexcept {
		library::bit_grid bit_grid(300, 300);
		bit_grid.set_bit(0, 0, true);
		//bit_grid.get_pos(div_t{ 0,0 });
	}
}