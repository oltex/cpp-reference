#pragma once
#include "map.h"
#include "../my_class.h"

#include <iostream>
#include <map>

#include <iostream>
#include <type_traits>
#include <utility>
#include <string>


template <typename type, typename... argument>
struct value_extract {
};
template <typename type, typename rest, typename... argument>
struct value_extract<type, rest, argument...> {
	static constexpr int count = std::is_constructible_v<type, rest, argument...> ? 1 + sizeof...(argument) : value_extract<type, argument...>::count;
};
template <typename type>
struct value_extract<type> {
	static constexpr int count = 0;
};

//static constexpr bool constructible = std::is_constructible_v<type, rest, argument...> ? true : value_extract<type, argument...>::constructible;

class my_class2 {
public:
	my_class2(int a, int b)
		: _a(a), _b(b) {
	}
	int _a;
	int _b;
};

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::map<int, int> a;
	//std_map.emplace(1, 2);
	//std_map.emplace(std::piecewise_construct,
	//	std::forward_as_tuple(1),
	//	std::forward_as_tuple(1));
	//std::forward_as_tuple()
	library::data_structure::map<int, my_class2> map2;
	map2.emplace2(8, 8, 7);


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