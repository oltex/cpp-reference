#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include "memory_pool.h"
#include "../../my_class.h"
#include "../../lockfree/queue/queue.h"
#include "../../lockfree/stack/stack.h"
#include <thread>
#include <vector>


data_structure::lockfree::queue<int*> _queue;
data_structure::lockfree::stack<int*> _stack;
extern volatile long _size1;

inline static unsigned int __stdcall func(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	std::vector<int*> _vector;

	for (;;) {
		std::cout << "allocate" << std::endl;
		for (auto i = 0; i < 40; ++i) {
			_vector.emplace_back(&instance.allocate(10));
		}
		for (auto i = 0; i < 40; ++i) {
			if (11 != ++(*_vector[i]))
				__debugbreak();
		}
		std::cout << "deallocate" << std::endl;
		for (auto i = 0; i < 40; ++i) {
			int* a = _vector.back();
			instance.deallocate(*a);
			_vector.pop_back();
		}
	}
	return 0;
}
inline static unsigned int __stdcall func1(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	for (;;) {
		//std::cout << "allocate" << std::endl;
		//for (auto i = 0; i < 200; ++i) {

		Sleep(0);
		int* value = &instance.allocate(20);
		(*value)++;
		_stack.push(value);
		InterlockedIncrement(&_size1);
		//}
	}
	return 0;
}
inline static unsigned int __stdcall func2(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();

	for (;;) {
		auto value = _stack.pop();
		if (value) {
			if (*(*value) != 21)
				__debugbreak();
			InterlockedDecrement(&_size1);
			//std::cout << "deallocate" << std::endl;
			instance.deallocate(*(*value));
		}
	}
	return 0;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
	std::vector<int*> _vector;

	//for (int i = 0; i < 50000; ++i) {
	//	_vector.emplace_back(&instance.allocate());
	//}
	//for (auto i = 0; i < 50000; ++i) {
	//	int* a = _vector.back();
	//	instance.deallocate(*a);
	//	_vector.pop_back();
	//}

	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func2, nullptr, 0, 0);
	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);

	//for (;;) {
	//	Sleep(1000);
	//	HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func2, nullptr, 0, 0);
	//}
	//HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}