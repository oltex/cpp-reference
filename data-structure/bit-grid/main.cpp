#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "bit_grid.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	library::data_structure::bit_grid<long long> bit_grid(300, 300);
	bit_grid.set_bit(0, 0, true);
	bit_grid.get_pos(div_t{ 0,0 });

	return 0;
}