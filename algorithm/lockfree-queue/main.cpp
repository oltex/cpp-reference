#include "lockfree_queue.h"
#include <thread>
#include <intrin.h>
#include <Windows.h>
#include <intrin.h>

lockfree_queue _lockfree_queue;
volatile unsigned int _value = 0;

inline static unsigned int __stdcall func(void* arg) noexcept {
	for (;;) {
		for (int i = 0; i < 5; ++i) {
			auto value = _InterlockedIncrement(&_value);
			_lockfree_queue.push(value);
		}
		for (int i = 0; i < 5; ++i) {
			_lockfree_queue.pop();
		}
	}
}

int main(void) noexcept {
	//_lockfree_queue.push(1);
	//auto a = _lockfree_queue.pop();

	HANDLE _handle0 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	HANDLE _handle1 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, nullptr, 0, 0));
	Sleep(INFINITE);
	return 0;
}