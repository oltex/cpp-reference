#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "segment_tree.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	library::data_structure::segment_tree<int> segment_tree{ 1, 4, -3 ,7,6,-2,0,1 };
	//segment_tree.update(2, 6);
	auto result = segment_tree.query(0, 2);
	//auto result2 = segment_tree.query2(0, 2);
	return 0;
}