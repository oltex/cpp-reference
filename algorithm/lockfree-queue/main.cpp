#include "lockfree_queue2.h"
#include <thread>
#include <intrin.h>
#include <Windows.h>
#include <intrin.h>
#include <iostream>

lockfree_queue _lockfree_queue;
volatile unsigned int _value = 0;

inline static unsigned int __stdcall func(void* arg) noexcept {
	int count = 0;
	for (;;) {
		if (count++ == 10000) {
			std::cout << "thread :" << GetCurrentThreadId() /*<< "size :" << _lockfree_queue._size */<< std::endl;
			count = 0;
		}
		for (int i = 0; i < 2; ++i) {
			//auto value = _InterlockedIncrement(&_value);
			_lockfree_queue.push(1);
		}
		for (int i = 0; i < 2; ++i) {
			_lockfree_queue.pop();
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