#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "vector.h"

#include <iostream>
#include <algorithm>
#include <vector>

class A {
public:
	A(int a, int b) 
		: _a(a), _b(b){
	}
public:
	int _a;
	int _b;
};

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::vector<my_class> std_vector;
	data_structure::vector<int> vector;
	vector.emplace_back(1);
	vector.emplace_back(2);
	vector.emplace_back(3);
	vector.emplace_back(4);
	vector.emplace_back(5);
	vector.emplace_back(6);
	//vector.clear();

	for (auto& iter : vector)
		std::cout << iter << std::endl;


	//for (auto iter = vector.begin(); iter != vector.end(); ++iter)
	//	(*iter)++;

	//std::cout << vector.front()._a << std::endl;
	//std::cout << vector.back()._a << std::endl;
	//std::cout << "===========" << std::endl;

	for (auto iter = vector.begin(); iter != vector.end(); ++iter)
		std::cout << (*iter) << std::endl;

	//vectortor<Class> vector2{ vector };
	//std::find(vector.begin(), vector.end(), 10);
	return 0;
}