#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "../pair/pair.h"
#include <iostream>
#include <tuple>
#include "tuple.h"
int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::tuple<int, int, int> std_tuple;
	std::tuple<my_class, my_class, my_class> std_tuple2(1, 2, 3);
	data_structure::tuple<my_class, int, int> tuple(1, 2, 3);
	data_structure::tuple<my_class, int, int> tuple2(4, 5, 6);
	tuple = std::move(tuple2);

	auto& [result, byte, key] = tuple;

	int b = byte;

	//data_structure::pair<int, int> p;
	////std::get<0>(p);
	//auto b = tuple.get<0>();
	//auto c = tuple2.get<0>();
	return 0;
}