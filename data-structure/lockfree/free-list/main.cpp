#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "free_array.h"
#include "../../my_class.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	library::lockfree::free_array<my_class> free_array;
	//free_array.reserve(10, 10);
	//bit_grid.get_pos(div_t{ 0,0 });

	return 0;
}