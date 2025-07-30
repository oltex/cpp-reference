#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "vector.h"

#include <iostream>
#include <algorithm>
#include <vector>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::vector<int> std_vector;
	//std_vector.emplace_back(1);
	//std_vector.emplace_back(2);
	//std_vector.emplace_back(3);
	//std_vector.emplace_back(4);
	//std_vector.emplace_back(5);
	auto std_iter = std_vector.begin();
	//std_iter++;
	//std_iter++;
	auto std_iter2 = std_vector.emplace(std_iter, 10);

	library::vector<int> vector;
	vector.emplace_back(1);
	vector.emplace_back(2);
	vector.emplace_back(3);
	vector.emplace_back(4);
	vector.emplace_back(5);
	vector.pop_back();

	auto iter = vector.begin();
	library::advance(iter, 2);
	//auto iter2 = vector.emplace(iter, 10);
	auto iter2 = vector.erase(iter);

	//library::advance(iter, -2);

	vector.resize(3, 1);

	library::vector<int> vector2;
	vector = std::move(vector);
	vector2.swap(vector);

	for (int i = 0; i < vector.size(); ++i) {
		std::cout << vector[i] << std::endl;
	}
	for (int i = 0; i < vector2.size(); ++i) {
		std::cout << vector2[i] << std::endl;
	}
	return 0;
}