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

	//library::unorder_set<int> set;
	//std::unordered_set<int> std_set;

	//for (int i = 0; i < 10000; ++i)
	//	set.emplace(i);
	//for (int i = 0; i < 10000; ++i)
	//	std_set.emplace(i);


	//if (set.bucket_count() != std_set.bucket_count())
	//	__debugbreak();

	//for (int i = 0; i < set.bucket_count(); ++i) {
	//	auto set_iter = set.begin(i);
	//	auto std_set_iter = std_set.begin(i);

	//	while (set_iter != set.end(i)) {
	//		if (*set_iter != *std_set_iter)
	//			__debugbreak();
	//		++set_iter;
	//		++std_set_iter;
	//	}
	//}

	//library::data_structure::unordered_map<int, int> map;
	//for (int i = 0; i < 10000; ++i)
	//	map.emplace(i, i);
	//std::unordered_map<int, int> std_map;
	//for (int i = 0; i < 10000; ++i)
	//	std_map.emplace(i, i);

	return 0;
}