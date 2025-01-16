#include "lockfree_queue(boost).h"
#include <thread>
#include <intrin.h>
#include <Windows.h>
#include <intrin.h>
#include <iostream>

lockfree_queue _lockfree_queue;
volatile unsigned int _value = 0;
inline static unsigned int __stdcall func(void* arg) noexcept {
	int count = 0;
	unsigned int prev_value = 0;
	for (;;) {
		if (count++ == 100000) {
			printf("thread : %d\n", GetCurrentThreadId());
			count = 0;
		}
		for (int i = 0; i < 2; ++i) {
			_lockfree_queue.push(_InterlockedIncrement(&_value));
		}
		for (int i = 0; i < 2; ++i) {
			auto value = _lockfree_queue.pop();
			if (prev_value >= value)
				__debugbreak();
			else
				prev_value = value;
		}
	}
}

inline static unsigned int __stdcall func_tailtail1(void* arg) noexcept {
	for (;;) {
		_lockfree_queue.push(1);
		_lockfree_queue.pop();
	}
}
inline static unsigned int __stdcall func_tailtail2(void* arg) noexcept {
	for (;;) {
		_lockfree_queue.pop();
		_lockfree_queue.push(1);
	}
}

inline static unsigned int __stdcall func_pop1(void* arg) noexcept {
	for (;;) {
		_lockfree_queue.pop();
	}
}
inline static unsigned int __stdcall func_pop2(void* arg) noexcept {
	for (;;) {
		_lockfree_queue.pop();
		_lockfree_queue.push(1);
	}
}

inline static unsigned int __stdcall func_push(void* arg) noexcept {
	unsigned long long count = 0;
	for (;;) {
		_lockfree_queue.push(count);
		count++;
	}
}
inline static unsigned int __stdcall func_pop(void* arg) noexcept {
	unsigned long long count = 0;
	for (;;) {
		auto a =_lockfree_queue.pop();
		if (a == 0) {

		}
		else if (count != a)
			__debugbreak();
		count = a + 1;
	}
}


int main(void) noexcept {
	HANDLE _handle0 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	HANDLE _handle1 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	HANDLE _handle2 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	HANDLE _handle3 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	HANDLE _handle4 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	//system("pause");
	//ResumeThread(_handle0);
	//ResumeThread(_handle1);
	Sleep(INFINITE);
	return 0;
}