#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "queue.h"
//#include "../../module/multi/spin.h"
#include <thread>
#include <intrin.h>
#include <iostream>

data_structure::lockfree::lockfree_queue<int> _lockfree_queue;
volatile unsigned int _value = 0;
//multi::lock::spin _spin;

inline static unsigned int __stdcall func(void* arg) noexcept {
	int count = 0;
	for (;;) {
		if (count++ == 1000000) {
			std::cout << "thread :" << GetCurrentThreadId() << /*"time :" << ctsc - tsc <<*/ std::endl;
			count = 0;
		}
		for (int i = 0; i < 2; ++i) {
			_lockfree_queue.push(1);
		}
		for (int i = 0; i < 2; ++i) {
			auto result = _lockfree_queue.pop();
			if (result)
				if (1 != *result)
					__debugbreak();
		}
	}
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
//		auto a = _lockfree_queue.pop();
//		if (a == 0) {
//
//		}
//		else if (count != a)
//			__debugbreak();
//		count = a + 1;
//	}
//}


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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