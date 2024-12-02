#include "lockfree_stack.h"
#include <thread>

data_structure::lockfree_stack<int> lockfree_stack;

inline static unsigned int __stdcall func(void* arg) noexcept {
	for (;;) {
		for (int i = 0; i < 10; ++i)
			lockfree_stack.push(i);
		for (int i = 0; i < 10; ++i)
			lockfree_stack.pop();
	}
}

int main(void) noexcept {
	//std::thread t1(func);
	//std::thread t2(func);

	HANDLE _handle = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle2 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);

	Sleep(INFINITE);
	return 0;
}