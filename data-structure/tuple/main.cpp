#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include <iostream>
#include <tuple>
#include "tuple.h"
int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::tuple<my_class, my_class, my_class> std_tuple(1, 2, 3);
	std::tuple<my_class, my_class, my_class> std_tuple2(1, 2, 3);
	data_structure::tuple<my_class, int, int> tuple(1, 2, 3);
	data_structure::tuple<my_class, int, int> tuple2(2, 3, 4);

	auto b = data_structure::get<0>(tuple);
	return 0;
}