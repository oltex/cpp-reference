#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "../../module/multi/spin.h"
#include "lockfree_stack.h"
#include <iostream>
#include <thread>
#include <vector>
#include <intrin.h>

//data_structure::lockfree::stack<int> _stack;
//volatile unsigned int _value = 0;
//multi::lock::spin _spin;
//
//inline static unsigned int __stdcall func(void* arg) noexcept {
//	std::vector<int*> _vector;
//	for (;;) {
//		//std::cout << GetCurrentThreadId() << std::endl;
//		for (int i = 0; i < 10; ++i) {
//			_stack.push(i);
//		}
//		for (int i = 0; i < 10; ++i) {
//			int result = _stack.pop();
//			_spin.lock();
//			std::cout << result << std::endl;
//			_spin.unlock();
//		}
//	}
//
//	return 0;
//}


data_structure::lockfree::stack<my_class> _stack2;
int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	//HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	//HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	//HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	//HANDLE _handle4 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	//Sleep(INFINITE);

	_stack2.push(10);
	auto a = _stack2.pop();

	int b = 10;
	int c = 20;
	return 0;
}