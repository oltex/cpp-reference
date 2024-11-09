#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "unordered_map.h"

#include <unordered_map>
#include <intrin.h>
#include <iostream>

struct my {
	int a;
	bool operator==(const my& other) const {
		return a == other.a;
	}
};

namespace std {
	template <>
	struct hash<my> {
		size_t operator()(const my& p) const {
			return 0;
		}
	};
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	data_structure::unordered_map<int, int> map;
	std::cout << sizeof(map);
	map.emplace(1, 1);
	map.emplace(1, 1);
	map.clear();
	for (int i = 0; i < 10; i++)
		map.emplace(i, 0);
	//map.emplace(1, 0);
	//map.clear();
	//map.emplace(2, 0);
	//map.emplace(3, 0);
	//map.erase(1);
	//map.erase(3);
	//map.erase(2);
	int sum = 0;
	for (int i = 0; i < 16; ++i) {
		int cnt = 0;
		for (auto iter = map.begin(i); iter != map.end(i); ++iter) {
			std::cout << (*iter)._first << std::endl;
			cnt++;
		}
		std::cout << i << "cnt :" << cnt << std::endl;
		sum += cnt;
	}
	std::cout << "sum :" << sum << std::endl;
	return 0;
}