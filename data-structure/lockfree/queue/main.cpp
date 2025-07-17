#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "queue.h"
#include "queue.normal.h"
#include <thread>
#include <intrin.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <Windows.h>
#include <set>

#pragma region test1
//inline static unsigned int __stdcall func(void* arg) noexcept {
//	library::lockfree::queue<std::pair<unsigned long, unsigned int>>& _queue = *(library::lockfree::queue<std::pair<unsigned long, unsigned int>>*)(arg);
//	int count = 0;
//	volatile unsigned int _value = 0;
//	volatile unsigned int _prev_value = 0;
//	for (auto index = 0; index < 1000000; ++index) {
//		if (count++ == 1000000) {
//			printf("thread : %d\n", GetCurrentThreadId());
//			count = 0;
//		}
//		for (int i = 0; i < 2; ++i) {
//			_queue.emplace(std::pair<int, unsigned int>(GetCurrentThreadId(), ++_value));
//		}
//		for (int i = 0; i < 2; ++i) {
//			auto result = _queue.pop();
//			if (result) {
//				if (GetCurrentThreadId() == (*result).first) {
//					if (_prev_value >= (*result).second)
//						__debugbreak();
//					else
//						_prev_value = (*result).second;
//				}
//			}
//			else
//				__debugbreak();
//		}
//	}
//	return 0;
//}
#pragma endregion

#pragma region tls
//library::data_structure::lockfree::queue<unsigned long long> _lockfree_queue_array[5];
//unsigned int _lockfree_queue_index = 0;
//inline static unsigned int __stdcall func(void* arg) noexcept {
//	unsigned int _id = _InterlockedIncrement(&_lockfree_queue_index) - 1;
//	volatile unsigned int _prev_value = 0;
//	volatile unsigned int _value = 0;
//	int count = 0;
//	for (;;) {
//		if (count++ == 1000) {
//			printf("thread : %d\n", GetCurrentThreadId());
//			count = 0;
//		}
//		for (int i = 0; i < 10000; ++i) {
//			_lockfree_queue_array[_id].emplace(GetCurrentThreadId());
//		}
//		for (int i = 0; i < 10000; ++i) {
//			auto result = _lockfree_queue_array[_id].pop();
//			if (result) {
//				if(GetCurrentThreadId() != (*result))
//					__debugbreak();
//				//if (_prev_value >= (*result))
//				//	__debugbreak();
//				//else
//				//	_prev_value = (*result);
//			}
//			else {
//				i--;
//				__debugbreak();
//			}
//		}
//	}
//	return 0;
//}
#pragma endregion

#pragma region push pop
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
#pragma endregion

#pragma region MyRegion
//inline static unsigned int __stdcall func(void* arg) noexcept {
//	library::data_structure::lockfree::queue<unsigned long long>& _lockfree_queue = *(library::data_structure::lockfree::queue<unsigned long long>*)(arg);
//	int count = 0;
//	auto rdtsc = __rdtsc();
//	unsigned long long _value = 0;
//	for (;;) {
//		if (count == 1000000) {
//			rdtsc = __rdtsc() - rdtsc;
//			printf("thread_push : %d, %lld\n", GetCurrentThreadId(), rdtsc);
//			rdtsc = __rdtsc();
//			count = 0;
//		}
//		for (int j = 0; j < 2; ++j)
//			_lockfree_queue.emplace(_value++);
//		for (int j = 0; j < 2;) {
//			auto result = _lockfree_queue.pop();
//			if (result) {
//				//if (_value > result)
//				//	__debugbreak();
//				//_value = *result;
//				j++;
//			}
//			else
//				__debugbreak();
//		}
//		count++;
//	}
//	return 0;
//}
#pragma endregion

#pragma region perfornamce
library::lockfree::queue<int> _lockfree_queue;
library::queue<int> _normal_queue;
std::queue<int> _std_queue;
alignas(64) SRWLOCK _srw;
alignas(64) CRITICAL_SECTION _cs;
LARGE_INTEGER _frequency;
inline static unsigned int __stdcall func1(void* arg) noexcept {
	LARGE_INTEGER _start;
	LARGE_INTEGER _end;
	long long _sum = 0;
	long long _count = 0;
	for (;;) {
		for (int i = 0; i < 100000; ++i) {
			for (auto j = 0; j < 500; ++j) {
				QueryPerformanceCounter(&_start);
				_lockfree_queue.emplace(0);
				QueryPerformanceCounter(&_end);
				_sum += _end.QuadPart - _start.QuadPart;
				//for (volatile int k = 0; k < 64; ++k) {
				//}
			}
			for (auto j = 0; j < 500; ++j) {
				QueryPerformanceCounter(&_start);
				_lockfree_queue.pop();
				QueryPerformanceCounter(&_end);
				_sum += _end.QuadPart - _start.QuadPart;
				//for (volatile int k = 0; k < 64; ++k) {
				//}
			}
		}
		++_count;
		printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
	}
	return 0;
}
inline static unsigned int __stdcall func2(void* arg) noexcept {
	LARGE_INTEGER _start;
	LARGE_INTEGER _end;
	long long _sum = 0;
	long long _count = 0;
	for (;;) {
		for (int i = 0; i < 100000; ++i) {
			for (auto j = 0; j < 500; ++j) {
				QueryPerformanceCounter(&_start);
				//EnterCriticalSection(&_cs);
				AcquireSRWLockExclusive(&_srw);
				_std_queue.push(0);
				ReleaseSRWLockExclusive(&_srw);
				//LeaveCriticalSection(&_cs);
				QueryPerformanceCounter(&_end);
				_sum += _end.QuadPart - _start.QuadPart;
				//for (volatile int k = 0; k < 64; ++k) {
				//}
			}
			for (auto j = 0; j < 500; ++j) {
				QueryPerformanceCounter(&_start);
				//EnterCriticalSection(&_cs);
				AcquireSRWLockExclusive(&_srw);
				_std_queue.pop();
				ReleaseSRWLockExclusive(&_srw);
				//LeaveCriticalSection(&_cs);
				QueryPerformanceCounter(&_end);
				_sum += _end.QuadPart - _start.QuadPart;
				//for (volatile int k = 0; k < 64; ++k) {
				//}
			}
		}
		++_count;
		printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
	}
	return 0;
}
#pragma endregion

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	library::lockfree::queue<unsigned int> _queue;
	//library::lockfree::queue<std::pair<unsigned long, unsigned int>> _queue;

	QueryPerformanceFrequency(&_frequency);
	InitializeSRWLock(&_srw);
	InitializeCriticalSection(&_cs);

	int count;
	scanf_s("%d", &count);
	for (int i = 0; i < count; ++i) {
		(HANDLE)_beginthreadex(nullptr, 0, func1, (void*)&_queue, 0, 0);
	}
	system("pause");
	return 0;
}