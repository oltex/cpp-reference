#include "lockfree_stack.h"
#include "lockfree_stack2.h"
#include "logger.h"
#include <thread>
#include <intrin.h>
#include <Windows.h>

logger& _logger = logger::instance();
lockfree_stack2 _lockfree_stack;
volatile unsigned int _value = 0;

inline static unsigned int __stdcall func(void* arg) noexcept {
	for (;;) {
		for (int i = 0; i < 10; ++i) {
			auto value = _InterlockedIncrement(&_value);
			//_logger.log(L"[thread : %05d] push start : %08d", GetCurrentThreadId(), value);
			_lockfree_stack.push(value);
			//_logger.log(L"[thread : %05d] push end   : %08d", GetCurrentThreadId(), value);
		}
		for (int i = 0; i < 10; ++i) {
			//_logger.log(L"[thread : %05d] pop  start", GetCurrentThreadId());
			_lockfree_stack.pop();
			//_logger.log(L"[thread : %05d] pop  end", GetCurrentThreadId());
		}
	}
}

int main(void) noexcept {
	//for (int i = 0; i < 100000000; ++i) {
	//	auto value = _InterlockedIncrement(&_value);
	//	_lockfree_stack.push(value);
	//}
	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}