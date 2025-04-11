#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "memory_pool.h"
#include <iostream>
#include <thread>
#include <vector>
#include <intrin.h>

library::data_structure::lockfree::memory_pool<int> _memory_pool;

inline static unsigned int __stdcall func(void* arg) noexcept {
	std::vector<int*> _vector;

	for (;;) {
		std::cout << "allocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			int* a = &_memory_pool.allocate();
			*a = 10;
			_vector.emplace_back(a);
		}
		for (auto i = 0; i < 5000; ++i) {
			if (11 != ++(*_vector[i]))
				__debugbreak();
		}
		std::cout << "deallocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			int* a = _vector.back();
			_memory_pool.deallocate(*a);
			_vector.pop_back();
		}
	}

	return 0;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//std::vector<int*> _vector;
	//for (int i = 0; i < 10000; ++i) {
	//	_vector.emplace_back(&_memory_pool.allocate());
	//}
	//for (auto i = 0; i < 10000; ++i) {
	//	int* a = _vector.back();
	//	_memory_pool.deallocate(*a);
	//	_vector.pop_back();
	//}

	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}