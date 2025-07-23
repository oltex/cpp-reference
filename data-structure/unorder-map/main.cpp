#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "unorder_map.h"
#include <unordered_map>

#include <intrin.h>
#include <iostream>
#include <Windows.h>


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::unordered_map<int, int> std_umap;
	std_umap.emplace(std::piecewise_construct, std::forward_as_tuple(10),std::forward_as_tuple(10));
	library::unorder_map<my_class, int> umap;
	umap.emplace(10, 10);

	std::pair<int, int> pair;
	library::construct(pair, 10, 10);


	return 0;
}