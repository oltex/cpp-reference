#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "tuple.h"
#include "../pair/pair.h"
#include "../my_class.h"
#include <iostream>
#include <tuple>

auto func(void) -> library::pair<int, int> {
	return library::pair<int, int>(10, 20);
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//std::_Tuple_get()
	std::tuple<int, int> std_tuple(2, 3);
	library::tuple<int, int> tuple(1, 2);
	//library::tuple<my_class, int, int> tuple2(std::move(tuple));

	int _a;
	int _b;
	library::tie(_a, _b) = func();

	//library::tuple<my_class, int, int> tuple2(4, 5, 6);
	//tuple = std::move(tuple2);
	//int test = tuple.get<1>();
	//auto& [result, byte, key] = tuple;

	//int b;
	//int c;
	//std::tie(b, c) = std_tuple;

	//library::data_structure::tuple<my_class, int, int> tuple2(4, 5, 6);
	//tuple = std::move(tuple2);


	//int b = byte;

	//library::data_structure::pair<int, int> p;
	////std::get<0>(p);
	//auto b = tuple.get<0>();
	//auto c = tuple2.get<0>();
	return 0;
}