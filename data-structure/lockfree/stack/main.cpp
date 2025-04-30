#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "stack.h"
#include "normal_stack.h"
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
		while (1 == _lock || 1 == _interlockedbittestandset(&_lock, 0)) {
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
	//		if(0 == _interlockedbittestandset(&_lock, 0))
	//			break;
	//	}
	//}
	inline void unlock(void) noexcept {
		_interlockedbittestandreset(&_lock, 0);
		//_lock = 0;
	}
public:
	alignas(64) volatile long _lock = 0;
};
class wait_on_address final {
public:
	inline explicit wait_on_address(void) noexcept = default;
	inline explicit wait_on_address(wait_on_address const& rhs) noexcept = delete;
	inline explicit wait_on_address(wait_on_address&& rhs) noexcept = delete;
	inline auto operator=(wait_on_address const& rhs) noexcept -> wait_on_address & = delete;
	inline auto operator=(wait_on_address&& rhs) noexcept -> wait_on_address & = delete;
	inline ~wait_on_address(void) noexcept = default;
public:
	inline void lock(void) noexcept {
		volatile long compare = 1;
		int cnt = 0;
		while (1 == _address || 1 == _InterlockedExchange(&_address, 1)) {
			if (cnt++ == 66) {
				cnt = 0;
				WaitOnAddress(&_address, (void*)&compare, sizeof(long), INFINITE);
			}
		}
	}
	inline void unlock(void) noexcept {
		_address = 0;
		WakeByAddressSingle((void*)&_address);
	}
private:
	volatile long _address = 0;
};

library::data_structure::lockfree::stack<int> _lockfree_stack;
library::data_structure::stack<int> _stack;
std::stack<int> _std_stack;
std::mutex _std_mutex;
spin _spin;
wait_on_address _wait;
SRWLOCK _srw;
LARGE_INTEGER _frequency;
int* _log = new int[30000000];
unsigned long long _logcnt = 0;

inline static unsigned int __stdcall func1(void* arg) noexcept {
	LARGE_INTEGER _start;
	LARGE_INTEGER _end;
	unsigned long long sum = 0;
	unsigned long long count = 0;
	int tid = GetCurrentThreadId();
	for (;;) {
		QueryPerformanceCounter(&_start);
		for (int j = 0; j < 1000; ++j) {
			for (int i = 0; i < 500; ++i) {
				_lockfree_stack.push(0);
				//_log[_logcnt++ % 30000000] = tid;
				//Sleep(0);
			}
			for (int i = 0; i < 500; ++i) {
				_lockfree_stack.pop();
				//_log[_logcnt++ % 30000000] = tid;
				//Sleep(0);
			}
		}
		QueryPerformanceCounter(&_end);
		sum += _end.QuadPart - _start.QuadPart;
		count++;
		printf("%f\n", (static_cast<double>(sum) / count) / static_cast<double>(_frequency.QuadPart) * 1e6);
	}
	return 0;
}
inline static unsigned int __stdcall func2(void* arg) noexcept {
	LARGE_INTEGER _start;
	LARGE_INTEGER _end;
	unsigned long long _sum = 0;
	unsigned long long _count = 0;
	int tid = GetCurrentThreadId();
	for (;;) {
		QueryPerformanceCounter(&_start);
		for (int j = 0; j < 1000; ++j) {
			for (int i = 0; i < 500; ++i) {
				AcquireSRWLockExclusive(&_srw);
				_stack.push(0);
				//_log[_logcnt++ % 30000000] = tid;
				ReleaseSRWLockExclusive(&_srw);
				//Sleep(0);
			}
			for (int i = 0; i < 500; ++i) {
				AcquireSRWLockExclusive(&_srw);
				_stack.pop();
				//_log[_logcnt++ % 30000000] = tid;
				ReleaseSRWLockExclusive(&_srw);
				//Sleep(0);
			}
		}
		QueryPerformanceCounter(&_end);
		_sum += _end.QuadPart - _start.QuadPart;
		_count++;
		printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e6);
	}
	return 0;
}

inline static unsigned int __stdcall func4(void* arg) noexcept {
	int a = 10;
	for (;;) {
		a++;
	}
	return 0;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	InitializeSRWLock(&_srw);
	QueryPerformanceFrequency(&_frequency);

	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	HANDLE _handle4 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	HANDLE _handle5 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	HANDLE _handle6 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	HANDLE _handle7 = (HANDLE)_beginthreadex(nullptr, 0, func1, nullptr, 0, 0);
	//HANDLE _handle8 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle9 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle10 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle11 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle12 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle13 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle14 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle15 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle16 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle17 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle18 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
	//HANDLE _handle19 = (HANDLE)_beginthreadex(nullptr, 0, func5, nullptr, 0, 0);
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