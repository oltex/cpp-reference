#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include "object_pool.h"
#include "../../my_class.h"
#include "../../lockfree/queue/queue.h"
#include "../../lockfree/stack/stack.h"
#include <thread>
#include <vector>

struct my_struct {
	unsigned long long _value[128];
};
data_structure::lockfree::stack<int*> _stack;
volatile unsigned long long _stack_size = 0;

inline static unsigned int __stdcall func_pool(void* arg) noexcept {
	auto& instance = data_structure::_thread_local::object_pool<my_struct>::instance();
	my_struct** _array = (my_struct**)malloc(sizeof(my_struct*) * 1000000);

	for (;;) {
		auto _rdtsc = __rdtsc();

		for (auto i = 0; i < 1000000; ++i) {
			if (instance.empty())
				_array[i] = &instance.allocate();
			else 
				_array[i] = &instance.acquire();
		}
		//for (auto i = 0; i < 1000000; ++i)
		//	if (11 != ++(*_array[i]))
		//		__debugbreak();
		for (auto i = 0; i < 1000000; ++i)
			instance.release(*_array[i]);

		auto result = __rdtsc() - _rdtsc;
		printf("pool : %llu\n", result);
	}
	return 0;
}
inline static unsigned int __stdcall func_new(void* arg) noexcept {
	my_struct** _array = (my_struct**)malloc(sizeof(my_struct*) * 1000000);

	for (;;) {
		auto _rdtsc = __rdtsc();

		for (auto i = 0; i < 1000000; ++i) {
			//_array[i] = new my_struct;
			_array[i] = (my_struct*)malloc(sizeof(my_struct));
		}
		//for (auto i = 0; i < 1000000; ++i)
		//	if (11 != ++(*_array[i]))
		//		__debugbreak();
		for (auto i = 0; i < 1000000; ++i) {
			//delete _array[i];
			free(_array[i]);
		}

		auto result = __rdtsc() - _rdtsc;
		printf("new : %llu\n", result);
	}
	return 0;
}

//inline static unsigned int __stdcall func_alloc(void* arg) noexcept {
//	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
//	for (;;) {
//		Sleep(0);
//		//std::cout << "allocate" << std::endl;
//		int* value = &instance.allocate(20);
//		(*value)++;
//		_stack.push(value);
//		InterlockedIncrement(&_stack_size);
//	}
//	return 0;
//}
//inline static unsigned int __stdcall func_dealloc(void* arg) noexcept {
//	auto& instance = data_structure::_thread_local::memory_pool<int>::instance();
//
//	for (;;) {
//		auto value = _stack.pop();
//		if (value) {
//			//std::cout << "deallocate" << std::endl;
//			if (*(*value) != 21)
//				__debugbreak();
//			InterlockedDecrement(&_stack_size);
//			instance.deallocate(*(*value));
//		}
//	}
//	return 0;
//}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//auto& instance = data_structure::_thread_local::object_pool<my_class>::instance();
	//std::vector<my_class*> _vector;
	//for (int i = 0; i < 4; ++i) {
	//	if (instance.empty())
	//		_vector.emplace_back(&instance.allocate(10));
	//	else {
	//		_vector.emplace_back(&instance.acquire());
	//	}
	//}
	//for (auto i = 0; i < 4; ++i) {
	//	my_class* a = _vector.back();
	//	instance.release(*a);
	//	_vector.pop_back();
	//}

	//HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func_new, nullptr, 0, 0);
	//HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func_new, nullptr, 0, 0);
	//HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func_new, nullptr, 0, 0);
	//HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func_new, nullptr, 0, 0);
	HANDLE _handle4 = (HANDLE)_beginthreadex(nullptr, 0, func_pool, nullptr, 0, 0);
	HANDLE _handle5 = (HANDLE)_beginthreadex(nullptr, 0, func_pool, nullptr, 0, 0);
	HANDLE _handle6 = (HANDLE)_beginthreadex(nullptr, 0, func_pool, nullptr, 0, 0);
	HANDLE _handle7 = (HANDLE)_beginthreadex(nullptr, 0, func_pool, nullptr, 0, 0);

	//HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func_dealloc, nullptr, 0, 0);
	//HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func_alloc, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}