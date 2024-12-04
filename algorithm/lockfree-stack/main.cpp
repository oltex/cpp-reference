#include "lockfree_stack.h"
#include "logger.h"
#include <thread>
#include <intrin.h>
#include <Windows.h>

logger& _logger = logger::instance();
lockfree_stack _lockfree_stack;
volatile unsigned int _value = 0;

inline static unsigned int __stdcall func(void* arg) noexcept {
	for (;;) {
		//for (int i = 0; i < 10; ++i) {
		//	auto value = _InterlockedIncrement(&_value);
		//	_lockfree_stack.push(value);
		//}
		for (int i = 0; i < 10; ++i) {
			_lockfree_stack.pop();
		}
	}
}

int main(void) noexcept {
	for (int i = 0; i < 10000000; ++i) {
		auto value = _InterlockedIncrement(&_value);
		_lockfree_stack.push(value);
	}
	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}