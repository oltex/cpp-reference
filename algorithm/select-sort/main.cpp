#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "select_sort.h"
#include "../../data-structure/list/list.h"
#include "../../data-structure/vector/vector.h"

#include <vector>
#include <list>
#include <algorithm>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::vector<int> std_vector;
	std::list<int> std_list;
	std_vector.push_back(1);
	std_vector.push_back(2);
	std::iter_swap(std_vector.begin(), std_vector.begin()++);


	library::vector<int> vector;
	library::list<int> list;
	vector.emplace_back(5);
	vector.emplace_back(2);
	vector.emplace_back(3);
	vector.emplace_back(4);
	vector.emplace_back(1);
	list.emplace_back(5);
	list.emplace_back(2);
	list.emplace_back(3);
	list.emplace_back(4);
	list.emplace_back(1);

	library::select_sort(list.begin(), list.end());


	return 0;
}