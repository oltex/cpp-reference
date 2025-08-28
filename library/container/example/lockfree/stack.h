#pragma once
#include "../../lockfree/stack.h"
#include "../my_class.h"
#include <thread>
#include <stack>
#include <mutex>
#include <Windows.h>

namespace example::lockfree {
	//library::stack<int> _stack;
	//std::stack<int> _std_stack;
	//std::mutex _std_mutex;
	//alignas(64) SRWLOCK _srw;
	//alignas(64) CRITICAL_SECTION cs;
	//int* _log = new int[30000000];
	//unsigned long long _logcnt = 0;
	////_log[_logcnt++ % 30000000] = _tid;
	LARGE_INTEGER _frequency;

	inline static unsigned int __stdcall stack_performance(void* arg) noexcept {
		auto& _stack = *reinterpret_cast<library::lockfree::stack<int>*>(arg);

		LARGE_INTEGER _start, _end;
		unsigned long long _sum = 0;
		unsigned long long _count = 0;
		int _tid = GetCurrentThreadId();
		for (;;) {
			QueryPerformanceCounter(&_start);
			for (int j = 0; j < 10000; ++j) {
				for (int i = 0; i < 500; ++i) {
					_stack.push(0);
				}
				for (int i = 0; i < 500; ++i) {
					_stack.pop();
				}
			}
			QueryPerformanceCounter(&_end);
			_sum += _end.QuadPart - _start.QuadPart;
			_count++;
			printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
		}
		return 0;
	}
	
	//inline static unsigned int __stdcall srwstack_perforamnce(void* arg) noexcept {
	//	LARGE_INTEGER _start, _end;
	//	unsigned long long _sum = 0;
	//	unsigned long long _count = 0;
	//	int _tid = GetCurrentThreadId();
	//	for (;;) {
	//		QueryPerformanceCounter(&_start);
	//		for (int j = 0; j < 10000; ++j) {
	//			for (int i = 0; i < 500; ++i) {
	//				//_spin.lock();
	//				//EnterCriticalSection(&cs);
	//				AcquireSRWLockExclusive(&_srw);
	//				//_stack.push(0);
	//				//_log[_logcnt++ % 30000000] = _tid;
	//				//LeaveCriticalSection(&cs);
	//				ReleaseSRWLockExclusive(&_srw);
	//				//_spin.unlock();
	//			}
	//			for (int i = 0; i < 500; ++i) {
	//				//_spin.lock();
	//				//EnterCriticalSection(&cs);
	//				AcquireSRWLockExclusive(&_srw);
	//				//_stack.pop();
	//				//_log[_logcnt++ % 30000000] = _tid;
	//				//LeaveCriticalSection(&cs);
	//				ReleaseSRWLockExclusive(&_srw);
	//				//_spin.unlock();
	//			}
	//		}
	//		QueryPerformanceCounter(&_end);
	//		_sum += _end.QuadPart - _start.QuadPart;
	//		_count++;
	//		printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
	//	}
	//	return 0;
	//}

	inline void stack(void) noexcept {
		library::lockfree::stack<int> _stack;
		//InitializeSRWLock(&_srw);
		//InitializeCriticalSection(&cs);
		QueryPerformanceFrequency(&_frequency);

		int count;
		scanf_s("%d", &count);
		for (int i = 0; i < 4; ++i)
			(HANDLE)_beginthreadex(nullptr, 0, stack_performance, reinterpret_cast<void*>(&_stack), 0, 0);
		system("pause");
	}
}



//#include <iostream>
//#include <vector>
//#include <intrin.h>

//library::data_structure::lockfree::stack<int> _stack;
//volatile unsigned int _value = 0;
//multi::lock::spin _spin;

//inline static unsigned int __stdcall func(void* arg) noexcept {
//	std::vector<int*> _vector;
//	int count = 0;
//	for (;;) {
//		if (count++ == 10000) {
//			std::cout << "thread :" << GetCurrentThreadId() /*<< "size :" << _lockfree_queue._size */ << std::endl;
//			count = 0;
//		}
//
//		for (int i = 0; i < 3; ++i) {
//			_stack.push(i);
//		}
//		for (int i = 0; i < 3; ++i) {
//			auto result = _stack.pop();
//			if (!result)
//				__debugbreak();
//			//_spin.lock();
//			//std::cout << result << std::endl;
//			//_spin.unlock();
//		}
//	}
//	return 0;
//}