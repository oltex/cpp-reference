#include "lockfree_stack.h"
#include <thread>
#include <intrin.h>
#include <Windows.h>
#include <intrin.h>

lockfree_stack _lockfree_stack;

inline static unsigned int __stdcall func(void* arg) noexcept {
	for (;;) {
		for (int i = 0; i < 2; ++i) {
			//auto value = _InterlockedIncrement(&_value);
			_lockfree_stack.push(0);
		}
		for (int i = 0; i < 2; ++i) {
			_lockfree_stack.pop();
		}
	}
}

int main(void) noexcept {
	//for (int i = 0; i < 10000000; ++i) {
	//	auto value = _InterlockedIncrement(&_value);
	//	_lockfree_stack.push(value);
	//}

	HANDLE _handle0 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	HANDLE _handle1 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	Sleep(INFINITE);
	return 0;
}