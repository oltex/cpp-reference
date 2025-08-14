#pragma once
#include "pool.h"
#include <iostream>
#include <thread>
#include <vector>
#include <intrin.h>

library::lockfree::pool<int> _pool;

inline static unsigned int __stdcall func(void* arg) noexcept {
	std::vector<int*> _vector;

	for (int j = 0; j < 1000; ++j) {
		std::cout << "allocate" << std::endl;
		for (auto i = 0; i < 2; ++i) {
			int* a = _pool.allocate();
			*a = 10;
			_vector.emplace_back(a);
		}
		for (auto i = 0; i < 2; ++i) {
			if (11 != ++(*_vector[i]))
				__debugbreak();
		}
		std::cout << "deallocate" << std::endl;
		for (auto i = 0; i < 2; ++i) {
			int* a = _vector.back();
			_pool.deallocate(a);
			_vector.pop_back();
		}
	}

	return 0;
}

int main(void) noexcept {
	//std::vector<int*> _vector;
	//for (int i = 0; i < 10000; ++i) {
	//	_vector.emplace_back(&_pool.allocate());
	//}
	//for (auto i = 0; i < 10000; ++i) {
	//	int* a = _vector.back();
	//	_pool.deallocate(*a);
	//	_vector.pop_back();
	//}

	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	system("pause");
	//Sleep(INFINITE);
	return 0;
}