#pragma once
#include <iostream>
#include "pool.h"
#include "../../my_class.h"
#include "../../lockfree/queue/queue.h"
#include "../../lockfree/stack/stack.h"
#include <thread>
#include <vector>

namespace example {
#pragma region debug
	//struct my_struct {
	//	my_struct(void) = delete;
	//	~my_struct(void) = delete;
	//	unsigned long long _value[128];
	//};

	//library::data_structure::lockfree::queue<int*> _queue;
	//library::data_structure::lockfree::stack<int*> _stack;
	//volatile unsigned long long _stack_size = 0;

	//inline static unsigned int __stdcall func_pool(void* arg) noexcept {
	//	auto& instance = library::data_structure::_thread_local::pool<my_struct>::instance();
	//	my_struct** _array = (my_struct**)malloc(sizeof(my_struct*) * 1000000);
	//
	//	for (int i = 0; i < 1000; ++i) {
	//		auto _rdtsc = __rdtsc();
	//
	//		for (auto i = 0; i < 200; ++i) {
	//			_array[i] = &instance.allocate();
	//		}
	//		//for (auto i = 0; i < 1000000; ++i)
	//		//	if (11 != ++(*_array[i]))
	//		//		__debugbreak();
	//		for (auto i = 0; i < 200; ++i) {
	//			instance.deallocate(*_array[i]);
	//		}
	//
	//		auto result = __rdtsc() - _rdtsc;
	//		printf("pool : %llu\n", result);
	//	}
	//
	//	free(_array);
	//	return 0;
	//}
	//inline static unsigned int __stdcall func_alloc(void* arg) noexcept {
	//	auto& instance = library::data_structure::_thread_local::memory_pool<int>::instance();
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
	//	auto& instance = library::data_structure::_thread_local::memory_pool<int>::instance();
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
#pragma endregion

	LARGE_INTEGER _frequency;
	long long _sum = 0;
	long long _count = 0;

	struct my_str {
		inline explicit my_str(void) noexcept = delete;
		inline ~my_str(void) noexcept = delete;
		unsigned char _buffer[256];
	};

	inline static unsigned int __stdcall func(void* arg) noexcept {
		auto& _pool = library::_thread_local::pool<my_str>::instance();
		my_str** _array = reinterpret_cast<my_str**>(malloc(sizeof(my_str*) * 10000));
		if (nullptr == _array)
			__debugbreak();
		LARGE_INTEGER _start;
		LARGE_INTEGER _end;
		for (;;) {
			QueryPerformanceCounter(&_start);
			for (int i = 0; i < 10000; ++i) {
				for (auto j = 0; j < 5000; ++j)
					_array[j] = _pool.allocate();
				for (auto j = 0; j < 5000; ++j)
					_pool.deallocate(_array[j]);
			}
			QueryPerformanceCounter(&_end);
			printf("%f\n", (_end.QuadPart - _start.QuadPart) /
				static_cast<double>(_frequency.QuadPart) * 1e3);
		}
		free(_array);
		return 0;
	}
	inline static unsigned int __stdcall func2(void* arg) noexcept {
		auto& _pool = library::_thread_local::pool<int>::instance();
		my_str** _array = reinterpret_cast<my_str**>(malloc(sizeof(my_str*) * 10000));
		if (nullptr == _array)
			__debugbreak();
		LARGE_INTEGER _start;
		LARGE_INTEGER _end;
		for (;;) {
			QueryPerformanceCounter(&_start);
			for (int i = 0; i < 10000; ++i) {
				for (auto j = 0; j < 5000; ++j)
					_array[j] = reinterpret_cast<my_str*>(malloc(sizeof(my_str)));
				for (auto j = 0; j < 5000; ++j)
					free(_array[j]);
			}
			QueryPerformanceCounter(&_end);
			printf("%f\n", (_end.QuadPart - _start.QuadPart) /
				static_cast<double>(_frequency.QuadPart) * 1e3);
		}
		free(_array);
		return 0;
	}

	int main(void) noexcept {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		QueryPerformanceFrequency(&_frequency);

		int count = 4;
		//scanf_s("%d", &count);
		for (int i = 0; i < count; ++i) {
			(HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
		}
		system("pause");
		return 0;
	}



	//inline static unsigned int __stdcall func_new(void* arg) noexcept {
	//	auto& instance = library::data_structure::_thread_local::memory_pool<int>::instance();
	//	my_struct** _array = (my_struct**)malloc(sizeof(my_struct*) * 1000000);
	//	for (;;) {
	//		auto _rdtsc = __rdtsc();
	//
	//		for (auto i = 0; i < 1000000; ++i) {
	//			//_array[i] = new my_struct;
	//			_array[i] = (my_struct*)malloc(sizeof(my_struct));
	//		}
	//		//for (auto i = 0; i < 1000000; ++i)
	//		//	if (11 != ++(*_array[i]))
	//		//		__debugbreak();
	//		for (auto i = 0; i < 1000000; ++i) {
	//			//delete _array[i];
	//			free(_array[i]);
	//		}
	//
	//		auto result = __rdtsc() - _rdtsc;
	//		printf("new : %llu\n", result);
	//	}
	//	return 0;
	//}
}