#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../queue/queue(boost).h"
#include "stack.h"
#include "normal_stack.h"
#include "normal_queue.h"
#include <thread>
#include <stack>
#include <mutex>
#include <Windows.h>

class spin final {
public:
	inline explicit spin(void) noexcept = default;
	inline explicit spin(spin const& rhs) noexcept = delete;
	inline explicit spin(spin&& rhs) noexcept = delete;
	inline auto operator=(spin const& rhs) noexcept -> spin & = delete;
	inline auto operator=(spin&& rhs) noexcept -> spin & = delete;
	inline ~spin(void) noexcept = default;
public:
	inline void lock(void) noexcept {
		while (/*1 == _lock ||*/ 1 == _InterlockedExchange(&_lock, 1)) {
			YieldProcessor();
		}
	}
	//inline void lock(void) noexcept {
	//	if (0 == _interlockedbittestandset(&_lock, 0)) {
	//		return;
	//	}
	//	for (;;) {
	//		while (1 == _lock) {
	//			YieldProcessor();
	//		}
	//		for (int k = 0; k < 10000; ++k) {
	//		}
	//		if (0 == _interlockedbittestandset(&_lock, 0))
	//			break;
	//	}
	//}
	inline void unlock(void) noexcept {
		_InterlockedExchange(&_lock, 0);
		//_lock = 0;
	}
public:
	alignas(64) long _lock = 0;
};

bool _run = false;
library::data_structure::lockfree::queue<int> _lockfree_queue;
library::data_structure::lockfree::stack<int> _lockfree_stack;
library::data_structure::stack<int> _stack;
library::data_structure::queue<int> _queue;
spin _spin;
std::stack<int> _std_stack;
std::mutex _std_mutex;
alignas(64) SRWLOCK _srw;
alignas(64) CRITICAL_SECTION cs;
LARGE_INTEGER _frequency;
int* _log = new int[30000000];
unsigned long long _logcnt = 0;
//_log[_logcnt++ % 30000000] = _tid;

inline static unsigned int __stdcall func1(void* arg) noexcept {
	while (!_run) {
	}
	LARGE_INTEGER _start;
	LARGE_INTEGER _end;
	unsigned long long _sum = 0;
	unsigned long long _count = 0;
	int _tid = GetCurrentThreadId();
	for (;;) {
		QueryPerformanceCounter(&_start);
		for (int j = 0; j < 10000; ++j) {
			for (int i = 0; i < 500; ++i) {
				_lockfree_stack.push(0);
				//_lockfree_queue.emplace(0);
			}
			for (int i = 0; i < 500; ++i) {
				_lockfree_stack.pop();
				//_lockfree_queue.pop();
			}
		}
		QueryPerformanceCounter(&_end);
		_sum += _end.QuadPart - _start.QuadPart;
		_count++;
		printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
	}
	return 0;
}


inline static unsigned int __stdcall func2(void* arg) noexcept {
	while (!_run) {
	}
	LARGE_INTEGER _start;
	LARGE_INTEGER _end;
	unsigned long long _sum = 0;
	unsigned long long _count = 0;
	int _tid = GetCurrentThreadId();
	for (;;) {
		QueryPerformanceCounter(&_start);
		for (int j = 0; j < 10000; ++j) {
			for (int i = 0; i < 500; ++i) {
				//_spin.lock();
				EnterCriticalSection(&cs);
				//AcquireSRWLockExclusive(&_srw);
				_stack.push(0);
				//_log[_logcnt++ % 30000000] = _tid;
				//_queue.push(0);
				LeaveCriticalSection(&cs);
				//ReleaseSRWLockExclusive(&_srw);
				//_spin.unlock();
			}
			for (int i = 0; i < 500; ++i) {
				//_spin.lock();
				EnterCriticalSection(&cs);
				//AcquireSRWLockExclusive(&_srw);
				_stack.pop();
				//_log[_logcnt++ % 30000000] = _tid;

				//_queue.pop();
				LeaveCriticalSection(&cs);
				//ReleaseSRWLockExclusive(&_srw);
				//_spin.unlock();
			}
		}
		QueryPerformanceCounter(&_end);
		_sum += _end.QuadPart - _start.QuadPart;
		_count++;
		printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
	}
	return 0;
}

inline static unsigned int __stdcall func3(void* arg) noexcept {
	int a = 10;
	for (;;) {
		a++;
	}
	return 0;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	InitializeSRWLock(&_srw);
	InitializeCriticalSection(&cs);
	QueryPerformanceFrequency(&_frequency);

	for (int i = 0; i < 4; ++i)
		(HANDLE)_beginthreadex(nullptr, 0, func2, nullptr, 0, 0);
	//for (int i = 0; i < 32; ++i)
	//	(HANDLE)_beginthreadex(nullptr, 0, func3, nullptr, 0, 0);
	system("pause");
	_run = true;
	Sleep(INFINITE);
	return 0;
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