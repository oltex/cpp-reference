#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "queue(boost).h"
#include <thread>
#include <intrin.h>
#include <iostream>


//library::data_structure::lockfree::queue<std::pair<unsigned long, unsigned int>> _lockfree_queue;
//inline static unsigned int __stdcall func(void* arg) noexcept {
//	int count = 0;
//	volatile unsigned int _value = 0;
//	volatile unsigned int _prev_value = 0;
//	for (;;/*int i = 0; i < 100; ++i*/) {
//		if (count++ == 1000000) {
//			printf("thread : %d\n", GetCurrentThreadId());
//			count = 0;
//		}
//		for (int i = 0; i < 2; ++i) {
//			_lockfree_queue.emplace(std::pair<int, unsigned int>(GetCurrentThreadId(), ++_value));
//		}
//		for (int i = 0; i < 2; ++i) {
//			auto result = _lockfree_queue.pop();
//			if (result) {
//				if (GetCurrentThreadId() == (*result).first) {
//					if (_prev_value >= (*result).second)
//						__debugbreak();
//					else
//						_prev_value = (*result).second;
//				}
//			}
//			else
//				i--;
//		}
//	}
//	return 0;
//}

//library::data_structure::lockfree::queue<unsigned long long> _lockfree_queue_array[5];
//unsigned int _lockfree_queue_index = 0;
//inline static unsigned int __stdcall func_tls(void* arg) noexcept {
//	unsigned int _id = _InterlockedIncrement(&_lockfree_queue_index) - 1;
//	volatile unsigned int _prev_value = 0;
//	volatile unsigned int _value = 0;
//	int count = 0;
//	for (;;) {
//		if (count++ == 100000) {
//			printf("thread : %d\n", GetCurrentThreadId());
//			count = 0;
//		}
//		for (int i = 0; i < 2; ++i) {
//			_lockfree_queue_array[_id].emplace(GetCurrentThreadId());
//		}
//		for (int i = 0; i < 2; ++i) {
//			auto result = _lockfree_queue_array[_id].pop();
//			if (result) {
//				if(GetCurrentThreadId() != (*result))
//					__debugbreak();
//				//if (_prev_value >= (*result))
//				//	__debugbreak();
//				//else
//				//	_prev_value = (*result);
//			}
//			else
//				i--;
//		}
//	}
//	return 0;
//}


//library::data_structure::lockfree::queue<unsigned long long> _lockfree_queue;
//inline static unsigned int __stdcall func_push(void* arg) noexcept {
//	int count = 0;
//	auto rdtsc = __rdtsc();
//	unsigned long long _value = 0;
//	for (int i = 0; i < 100000000; ++i) {
//		if (count == 1000000) {
//			rdtsc = __rdtsc() - rdtsc;
//			printf("thread_push : %d, %lld\n", GetCurrentThreadId(), rdtsc);
//			rdtsc = __rdtsc();
//			count = 0;
//		}
//		_lockfree_queue.emplace(_value++);
//		count++;
//	}
//	return 0;
//}
//
//inline static unsigned int __stdcall func_pop(void* arg) noexcept {
//	int count = 0;
//	auto rdtsc = __rdtsc();
//	unsigned long long _value = 0;
//	for (;;) {
//		if (count == 1000000) {
//			rdtsc = __rdtsc() - rdtsc;
//			printf("thread_pop  : %d, %lld\n", GetCurrentThreadId(), rdtsc);
//			rdtsc = __rdtsc();
//			count = 0;
//		}
//		//if (!_lockfree_queue.empty()) {
//		//	unsigned long long result = _lockfree_queue.pop();
//		//	//if (_value > result)
//		//	//	__debugbreak();
//		//	_value = result;
//		//	count++;
//		//}
//
//		auto result = _lockfree_queue.pop();
//		if (result) {
//			if (_value > result)
//				__debugbreak();
//			_value = *result;
//			count++;
//		}
//	}
//}

inline static unsigned int __stdcall func(void* arg) noexcept {
	library::data_structure::lockfree::queue<unsigned long long>& _lockfree_queue = *(library::data_structure::lockfree::queue<unsigned long long>*)(arg);
	int count = 0;
	auto rdtsc = __rdtsc();
	unsigned long long _value = 0;
	for (int i = 0; i < 10000000; ++i) {
		if (count == 1000000) {
			rdtsc = __rdtsc() - rdtsc;
			printf("thread_push : %d, %lld\n", GetCurrentThreadId(), rdtsc);
			rdtsc = __rdtsc();
			count = 0;
		}
		for (int j = 0; j < 2; ++j)
			_lockfree_queue.emplace(_value++);
		for (int j = 0; j < 2;) {
			auto result = _lockfree_queue.pop();
			if (result) {
				//if (_value > result)
				//	__debugbreak();
				//_value = *result;
				j++;
			}
		}


		count++;
	}
	return 0;
}



int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	library::data_structure::lockfree::queue<unsigned long long> _lockfree_queue;

	HANDLE _handle0 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	HANDLE _handle1 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	HANDLE _handle2 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	HANDLE _handle3 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	//HANDLE _handle3 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func_pop, nullptr, 0, 0));
	//HANDLE _handle4 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func_pop, nullptr, 0, 0));
	//HANDLE _handle4 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	system("pause");
	//ResumeThread(_handle0);
	//ResumeThread(_handle1);
	//Sleep(INFINITE);
	return 0;
}