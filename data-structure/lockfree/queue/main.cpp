#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "queue(boost).h"
#include <thread>
#include <intrin.h>
#include <iostream>


inline static unsigned int __stdcall func(void* arg) noexcept {
	auto& _lockfree_queue = *(data_structure::lockfree::queue<std::pair<unsigned long, unsigned int>>*)(arg);
	int count = 0;
	volatile unsigned int _value = 0;
	volatile unsigned int _prev_value = 0;
	for (int i = 0; i < 100; ++i) {
		if (count++ == 1000000) {
			printf("thread : %d\n", GetCurrentThreadId());
			count = 0;
		}
		for (int i = 0; i < 2000; ++i) {
			_lockfree_queue.push(std::pair<int, unsigned int>(GetCurrentThreadId(), ++_value));
		}
		for (int i = 0; i < 2000; ++i) {
			auto result = _lockfree_queue.pop();
			if (result) {
				if (GetCurrentThreadId() == (*result).first) {
					if (_prev_value >= (*result).second)
						__debugbreak();
					else
						_prev_value = (*result).second;
				}
			}
			else
				i--;
		}
	}
	return 0;
}

//data_structure::lockfree::queue<unsigned int> _lockfree_queue_array[5];
//unsigned int _lockfree_queue_index = 0;
//inline static unsigned int __stdcall func_tls(void* arg) noexcept {
//	unsigned int _id = _InterlockedIncrement(&_lockfree_queue_index) - 1;
//	volatile unsigned int _prev_value = 0;
//	volatile unsigned int _value = 0;
//	int count = 0;
//	for (;;) {
//		if (count++ == 1000) {
//			printf("thread : %d\n", GetCurrentThreadId());
//			count = 0;
//		}
//		for (int i = 0; i < 1024; ++i) {
//			_lockfree_queue_array[_id].push(++_value);
//		}
//		for (int i = 0; i < 1024; ++i) {
//			auto result = _lockfree_queue_array[_id].pop();
//			if (result) {
//				if (_prev_value >= (*result))
//					__debugbreak();
//				else
//					_prev_value = (*result);
//			}
//			else
//				i--;
//		}
//	}
//	return 0;
//}
int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	data_structure::lockfree::queue<std::pair<unsigned long, unsigned int>> _lockfree_queue;

	HANDLE _handle0 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	HANDLE _handle1 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	HANDLE _handle2 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	HANDLE _handle3 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	HANDLE _handle4 = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, func, (void*)&_lockfree_queue, 0, 0));
	system("pause");
	//ResumeThread(_handle0);
	//ResumeThread(_handle1);
	//Sleep(INFINITE);
	return 0;
}