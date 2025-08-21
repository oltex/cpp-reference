#include "lockfree_queue.h"
#include <thread>
#include <intrin.h>
#include <Windows.h>
#include <intrin.h>
#include <iostream>

lockfree_queue _lockfree_queue;

inline static unsigned int __stdcall func(void* arg) noexcept {
	int count = 0;
	for (;;) {
		if (count++ == 100000) {
			printf("thread : %d\n", GetCurrentThreadId());
			count = 0;
		}
		for (int i = 0; i < 2; ++i)
			_lockfree_queue.push(0);
		for (int i = 0; i < 2; ++i)
			auto value = _lockfree_queue.pop();
	}
}

int main(void) noexcept {
	_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}

//inline static unsigned int __stdcall func_tailtail1(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.push(1);
//		_lockfree_queue.pop();
//	}
//}
//inline static unsigned int __stdcall func_tailtail2(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.pop();
//		_lockfree_queue.push(1);
//	}
//}
//
//inline static unsigned int __stdcall func_pop1(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.pop();
//	}
//}
//inline static unsigned int __stdcall func_pop2(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.pop();
//		_lockfree_queue.push(1);
//	}
//}
//
//inline static unsigned int __stdcall func_push(void* arg) noexcept {
//	unsigned long long count = 0;
//	for (;;) {
//		_lockfree_queue.push(count);
//		count++;
//	}
//}
//inline static unsigned int __stdcall func_pop(void* arg) noexcept {
//	unsigned long long count = 0;
//	for (;;) {
//		auto a =_lockfree_queue.pop();
//		if (a == 0) {
//
//		}
//		else if (count != a)
//			__debugbreak();
//		count = a + 1;
//	}
//}