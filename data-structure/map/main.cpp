#define _CRTDBGmap_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "map.h"
#include "../my_class.h"

#include <iostream>
#include <map>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::map<int, int> std_map;

	library::data_structure::map<int, my_class> map;
	map.emplace(8, 8);
	map.emplace(4, 4);
	map.emplace(2, 2);
	map.emplace(1, 1);
	map.emplace(3, 3);
	map.emplace(6, 6);
	map.emplace(5, 5);
	map.emplace(7, 7);

	map.erase(8);
	//map.erase(4);
	//map.erase(2);
	//map.erase(1);
	//map.erase(3);
	//map.erase(6);
	//map.erase(5);
	//map.erase(7);

	for (auto& iter : map)
		std::cout << iter._first << std::endl;

	library::data_structure::map<int, my_class>::iterator begin = map.begin();

	auto iter = map.find(8);
	if (iter != map.end())
		std::cout << "find" << std::endl;

	return 0;
}