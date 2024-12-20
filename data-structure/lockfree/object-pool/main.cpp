#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "object_pool.h"
#include <iostream>
#include <thread>
#include <vector>
#include <intrin.h>

data_structure::lockfree::object_pool<int> _object_pool;

inline static unsigned int __stdcall func(void* arg) noexcept {
	std::vector<int*> _vector;
	for (;;) {
		std::cout << GetCurrentThreadId() << std::endl;
		for (auto i = 0; i < 2000; ++i) {
			_vector.emplace_back(&_object_pool.acquire());
		}
		for (auto i = 0; i < 2000; ++i) {
			if (11 != ++(*_vector[i]))
				__debugbreak();
		}
		for (auto i = 0; i < 2000; ++i) {
			if (10 != --(*_vector[i]))
				__debugbreak();
		}
		for (auto i = 0; i < 2000; ++i) {
			int* a = _vector.back();
			_object_pool.release(*a);
			_vector.pop_back();
		}
	}

	return 0;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::vector<int*> _vector;
	for (int i = 0; i < 10000; ++i) {
		_vector.emplace_back(&_object_pool.acquire(10));
	}
	for (auto i = 0; i < 10000; ++i) {
		int* a = _vector.back();
		_object_pool.release(*a);
		_vector.pop_back();
	}

	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle4 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}