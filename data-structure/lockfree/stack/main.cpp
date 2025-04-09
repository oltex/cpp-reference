#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "../my_class.h"
//#include "../../module/multi/spin.h"
#include "stack.h"
#include <iostream>
#include <thread>
#include <vector>
#include <intrin.h>

library::data_structure::lockfree::stack<int> _stack;
volatile unsigned int _value = 0;
//multi::lock::spin _spin;

inline static unsigned int __stdcall func(void* arg) noexcept {
	std::vector<int*> _vector;
	int count = 0;
	for (;;) {
		if (count++ == 10000) {
			std::cout << "thread :" << GetCurrentThreadId() /*<< "size :" << _lockfree_queue._size */ << std::endl;
			count = 0;
		}

		for (int i = 0; i < 3; ++i) {
			_stack.push(i);
		}
		for (int i = 0; i < 3; ++i) {
			auto result = _stack.pop();
			if (!result)
				__debugbreak();
			//_spin.lock();
			//std::cout << result << std::endl;
			//_spin.unlock();
		}
	}

	return 0;
}


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle3 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle4 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}