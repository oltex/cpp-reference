#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "thread.h"
#include "spin_lock.h"
#include "wait_on_address_lock.h"

#include <thread>
#include <iostream>
#include <conio.h>

thread::wait_on_address_lock _lock;

bool wake = false;
bool wake2 = false;
long lock = 0;
long inter = 0;

void function(void) noexcept {
	volatile long compare = 0;
	while (true) {

		WaitOnAddress(&lock, (void*)&compare, sizeof(long), INFINITE);
		std::cout << "A" << std::endl;
	}
}


void function2(void) noexcept {
	while (true) {
		if (true == wake) {
			while (1 == _InterlockedExchange(&inter, 1)) {}
			WakeByAddressSingle((void*)&lock);
			std::cout << "B" << std::endl;
			wake = false;
			inter = 0;
		}
	}
}

void function3(void) noexcept {
	while (true) {
		if (true == wake2) {
			MemoryBarrier();
			WakeByAddressAll((void*)&lock);
			std::cout << "C" << std::endl;
			wake2 = false;
			MemoryBarrier();
		}
	}
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//{
	//	HANDLE handle[2];
	//	handle[0] = CreateThread(nullptr, 0, function, nullptr, CREATE_SUSPENDED, nullptr);
	//	handle[1] = CreateThread(nullptr, 0, function, nullptr, CREATE_SUSPENDED, nullptr);

	//	//SetThreadAffinityMask(handle[0], 1 << 0);
	//	//SetThreadAffinityMask(handle[1], 1 << 2);
	//	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	//	//SetThreadPriority(handle[0], THREAD_PRIORITY_TIME_CRITICAL);
	//	//SetThreadPriority(handle[1], THREAD_PRIORITY_TIME_CRITICAL);

	//	system("pause");
	//	auto rdtsc = __rdtsc();
	//	ResumeThread(handle[0]);
	//	ResumeThread(handle[1]);
	//	WaitForMultipleObjects(2, handle, true, INFINITE);
	//	std::cout << __rdtsc() - rdtsc << " : " << _value << std::endl;
	//}

	{
		thread::thread t1(function2, CREATE_SUSPENDED);
		thread::thread t2(function2, CREATE_SUSPENDED);
		thread::thread t3(function, CREATE_SUSPENDED);
		thread::thread t4(function, CREATE_SUSPENDED);
		thread::thread t5(function, CREATE_SUSPENDED);
		thread::thread t6(function, CREATE_SUSPENDED);
		thread::thread t7(function, CREATE_SUSPENDED);
		thread::thread t8(function, CREATE_SUSPENDED);
		thread::thread t9(function, CREATE_SUSPENDED);
		thread::thread t10(function, CREATE_SUSPENDED);
		system("pause");

		t1.resume();
		t2.resume();
		t3.resume();
		t4.resume();
		t5.resume();
		t6.resume();
		t7.resume();
		t8.resume();
		t9.resume();
		t10.resume();

		while (true) {
			int ch = _getch();
			if ('a' == ch)
				wake = true;
			if ('s' == ch)
				wake2 = true;
		}

		Sleep(INFINITE);

	}

	return 0;
}