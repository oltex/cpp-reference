#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "memory_pool.h"
#include "../../my_class.h"
#include <iostream>
#include <thread>
#include <vector>

inline static unsigned int __stdcall func(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	std::vector<int*> _vector;

	for (;;) {
		std::cout << "allocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			_vector.emplace_back(&instance.allocate(10));
		}
		for (auto i = 0; i < 5000; ++i) {
			if (11 != ++(*_vector[i]))
				__debugbreak();
		}
		std::cout << "deallocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			int* a = _vector.back();
			instance.deallocate(*a);
			_vector.pop_back();
		}
	}
	return 0;
}
inline static unsigned int __stdcall func1(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	std::vector<int*> _vector;

	for (;;) {
		std::cout << "allocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			_vector.emplace_back(&instance.allocate(20));
		}
		for (auto i = 0; i < 5000; ++i) {
			if (21 != ++(*_vector[i]))
				__debugbreak();
		}
		std::cout << "deallocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			int* a = _vector.back();
			instance.deallocate(*a);
			_vector.pop_back();
		}
	}
	return 0;
}
inline static unsigned int __stdcall func2(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	std::vector<int*> _vector;

	for (;;) {
		std::cout << "allocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			_vector.emplace_back(&instance.allocate(30));
		}
		for (auto i = 0; i < 5000; ++i) {
			if (31 != ++(*_vector[i]))
				__debugbreak();
		}
		std::cout << "deallocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			int* a = _vector.back();
			instance.deallocate(*a);
			_vector.pop_back();
		}
	}
	return 0;
}
inline static unsigned int __stdcall func3(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	std::vector<int*> _vector;

	for (;;) {
		std::cout << "allocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			_vector.emplace_back(&instance.allocate(40));
		}
		for (auto i = 0; i < 5000; ++i) {
			if (41 != ++(*_vector[i]))
				__debugbreak();
		}
		std::cout << "deallocate" << std::endl;
		for (auto i = 0; i < 5000; ++i) {
			int* a = _vector.back();
			instance.deallocate(*a);
			_vector.pop_back();
		}
	}
	return 0;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	std::vector<int*> _vector;

	for (int i = 0; i < 8; ++i) {
		_vector.emplace_back(&instance.allocate());
	}
	for (auto i = 0; i < 8; ++i) {
		int* a = _vector.back();
		instance.deallocate(*a);
		_vector.pop_back();
	}

	//HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	//HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	//HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func2, nullptr, 0, 0);
	//HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func3, nullptr, 0, 0);
	//Sleep(INFINITE);
	return 0;
}

//int main(void) noexcept {
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	HANDLE _handle0 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
//
//	auto& a = instance.allocate(1);
//	instance.deallocate(a);
//
//
//	return 0;
//}