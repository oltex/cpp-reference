#pragma once
#include "../unorder_map.h"
#include <unordered_map>
#include "my_class.h"

#include <intrin.h>
#include <iostream>
#include <Windows.h>


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::unordered_map<int, int> std_umap;
	//std_umap.emplace(std::piecewise_construct, std::forward_as_tuple(10), std::forward_as_tuple(10));
	library::unorder_map<my_class, int> umap;
	//umap.emplace(library::piecewise_construct, library::forward_as_tuple(10), library::forward_as_tuple(10));


	for (int i = 0; i < 10000; ++i)
		umap.emplace(i, i);
	umap.clear();
	for (int i = 0; i < 10000; ++i)
		umap.emplace(library::piecewise_construct, library::forward_as_tuple(i), library::forward_as_tuple(i));
	for (int i = 0; i < 10000; ++i)
		std_umap.emplace(i, i);
	std_umap.clear();
	for (int i = 0; i < 10000; ++i)
		std_umap.emplace(i, i);

	if (umap.bucket_count() != std_umap.bucket_count())
		__debugbreak();

	for (int i = 0; i < umap.bucket_count(); ++i) {
		auto set_iter = umap.begin(i);
		auto std_set_iter = std_umap.begin(i);

		while (set_iter != umap.end(i)) {
			if (set_iter->_first._value != std_set_iter->first)
				__debugbreak();
			++set_iter;
			++std_set_iter;
		}
	}

	return 0;
}