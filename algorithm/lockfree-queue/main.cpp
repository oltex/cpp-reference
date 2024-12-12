#include "lockfree_queue_old.h"
#include <thread>
#include <intrin.h>
#include <Windows.h>
#include <intrin.h>
#include <iostream>

lockfree_queue _lockfree_queue;
volatile unsigned int _value = 0;

inline static unsigned int __stdcall func(void* arg) noexcept {
	for (;;) {
		//std::cout << GetCurrentThreadId() << std::endl;
		for (int i = 0; i < 3; ++i) {
			auto value = _InterlockedIncrement(&_value);
			_lockfree_queue.push(value);
		}
		for (int i = 0; i < 3; ++i) {
			_lockfree_queue.pop();
		}
	}
}

inline static unsigned int __stdcall func1(void* arg) noexcept {
	for (;;) {
		_lockfree_queue.push(1);
		_lockfree_queue.pop();
	}
}
inline static unsigned int __stdcall func2(void* arg) noexcept {
	for (;;) {
		_lockfree_queue.pop();
		_lockfree_queue.push(1);
		_lockfree_queue.pop();
		_lockfree_queue.push(1);

		_lockfree_queue.pop();
		_lockfree_queue.pop();
		_lockfree_queue.push(1);
	}
}

int main(void) noexcept {
	_lockfree_queue.push(1);


	HANDLE _handle0 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func1, nullptr, 0, 0));
	HANDLE _handle1 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func2, nullptr, 0, 0));
	//HANDLE _handle2 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	//HANDLE _handle3 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	//HANDLE _handle4 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	Sleep(INFINITE);
	return 0;
}