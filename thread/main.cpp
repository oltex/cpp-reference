#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "thread.h"
#include "spin_lock.h"
#include "wait_on_address_lock.h"
#include "peterson_algorithm.h"

#include <thread>
#include <iostream>

thread::spin_lock _lock;
char padding[64];
thread::peterson_algorithm _peterson;
unsigned int _value = 0;

DWORD WINAPI function(LPVOID lparam) {
	for (int i = 0; i < 10000000; ++i) {
		_lock.lock();
		_value++;
		_lock.unlock();
	}
	return 0;
}

DWORD WINAPI peterson0(LPVOID lparam) {
	for (int i = 0; i < 10000000; ++i) {
		_peterson.lock<0>();
		_lock.lock();
		_value++;
		_lock.unlock();
		_peterson.unlock<0>();
	}
	return 0;
}
DWORD WINAPI peterson1(LPVOID lparam) {
	for (int i = 0; i < 10000000; ++i) {
		_peterson.lock<1>();
		_lock.lock();
		_value++;
		_lock.unlock();
		_peterson.unlock<1>();
	}
	return 0;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HANDLE handle[2];

	handle[0] = CreateThread(nullptr, 0, peterson0, nullptr, CREATE_SUSPENDED, nullptr);
	handle[1] = CreateThread(nullptr, 0, peterson1, nullptr, CREATE_SUSPENDED, nullptr);

	//SetThreadAffinityMask(handle[0], 1 << 0);
	//SetThreadAffinityMask(handle[1], 1 << 2);
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	//SetThreadPriority(handle[0], THREAD_PRIORITY_TIME_CRITICAL);
	//SetThreadPriority(handle[1], THREAD_PRIORITY_TIME_CRITICAL);

	system("pause");
	auto rdtsc = __rdtsc();
	ResumeThread(handle[0]);
	ResumeThread(handle[1]);
	WaitForMultipleObjects(2, handle, true, INFINITE);
	std::cout << __rdtsc() - rdtsc << " : " << _value << std::endl;

	return 0;
}