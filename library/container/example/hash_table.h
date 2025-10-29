#pragma once
#include "../hash_table.h"
#include "../string.h"
#include "my_class.h"

#include <unordered_set>
#include <unordered_map>
#include <intrin.h>
#include <iostream>
#include <Windows.h>

namespace example {
	inline void hash_table(void) noexcept {
		std::unordered_multimap<int, int> std_ummap;
		for (unsigned int i = 0; i < 100; ++i)
			std_ummap.emplace(rand() % 10, 100 - i);
		for (unsigned int i = 0; i < std_ummap.bucket_count(); ++i) {
			auto std_set_iter = std_ummap.begin(i);
			printf("bucket : %d\n", i);
			while (std_set_iter != std_ummap.end(i)) {
				printf("%d : %d\n", std_set_iter->first, std_set_iter->second);
				++std_set_iter;
			}
		}
		//library::unorder_multimap<int, int> ummap;
		//for (unsigned int i = 0; i < 100; ++i)
		//	ummap.emplace(rand() % 10, 100 - i);
		////for (unsigned int i = 0; i < 1000; ++i)
		////	ummap.emplace(rand() % 100, 1000 - i);
		//for (unsigned int i = 0; i < ummap.bucket_count(); ++i) {
		//	auto iter = ummap.begin(i);
		//	printf("bucket : %d\n", i);
		//	while (iter != ummap.end(i)) {
		//		printf("%d : %d\n", iter->_first, iter->_second);
		//		++iter;
		//	}
		//}




		//library::unorder_set<library::string> string_set;
		//string_set.emplace("hello");
		//string_set.find("hello");

		//library::unorder_set<int> set;
		//std::unordered_set<int> std_set;
		//for (int i = 0; i < 10000; ++i)
		//	set.emplace(i);
		//for (int i = 0; i < 10000; ++i)
		//	std_set.emplace(i);
		//if (set.bucket_count() != std_set.bucket_count())
		//	__debugbreak();

		//for (unsigned int i = 0; i < set.bucket_count(); ++i) {
		//	auto set_iter = set.begin(i);
		//	auto std_set_iter = std_set.begin(i);

		//	while (set_iter != set.end(i)) {
		//		if (*set_iter != *std_set_iter)
		//			__debugbreak();
		//		++set_iter;
		//		++std_set_iter;
		//	}
		//}


		//library::unorder_map<my_class, int> umap;
		//std::unordered_map<int, int> std_umap;
		////std_umap.emplace(std::piecewise_construct, std::forward_as_tuple(10), std::forward_as_tuple(10));
		////umap.emplace(library::piecewise_construct, library::forward_as_tuple(10), library::forward_as_tuple(10));
		//for (unsigned int i = 0; i < 10000; ++i)
		//	umap.emplace(i, i);
		//for (unsigned int i = 0; i < 10000; ++i)
		//	std_umap.emplace(i, i);
		//if (umap.bucket_count() != std_umap.bucket_count())
		//	__debugbreak();
		//for (unsigned int i = 0; i < umap.bucket_count(); ++i) {
		//	auto set_iter = umap.begin(i);
		//	auto std_set_iter = std_umap.begin(i);

		//	while (set_iter != umap.end(i)) {
		//		if (set_iter->_first._value != std_set_iter->first)
		//			__debugbreak();
		//		++set_iter;
		//		++std_set_iter;
		//	}
		//}

	}
}