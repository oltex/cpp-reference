#pragma once
#include "../bubble_sort.h"
#include "../../container/list.h"
#include "../../container/vector.h"

#include <vector>
#include <list>
#include <algorithm>

namespace example {
	inline void bubble_sort(void) noexcept {
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

		library::bubble_sort(list.begin(), list.end());
		int a = 10;
	}
}