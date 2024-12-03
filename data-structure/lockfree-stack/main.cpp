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
	HANDLE _handle0 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	HANDLE _handle1 = (HANDLE)_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}