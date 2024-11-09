#include "../../module/multi/spin_lock.h"
#include "peterson.h"

#include <thread>
#include <iostream>
#include <Windows.h>
#include <intrin.h>

peterson _peter;
multi::spin_lock _spin;
unsigned long long _value = 0;
long inter = 0;

DWORD WINAPI function_0(LPVOID lparam) {
	for (int i = 0; i < 100000000; ++i) {
		_peter.lock<0>();
		//_spin.lock();
		//if (0x00 != InterlockedOr(&inter, 0x01))
		//	__debugbreak();
		_value++;
		//if (0x01 != InterlockedAnd(&inter, ~0x01))
		//	__debugbreak();
		//_spin.unlock();
		_peter.unlock<0>();
	}
	return 0;
}
DWORD WINAPI function_1(LPVOID lparam) {
	for (int i = 0; i < 100000000; ++i) {
		_peter.lock<1>();
		//_spin.lock();
		//if (0x00 != InterlockedOr(&inter, 0x02))
		//	__debugbreak();
		_value++;
		//if (0x02 != InterlockedAnd(&inter, ~0x02))
		//	__debugbreak();
		//_spin.unlock();
		_peter.unlock<1>();
	}
	return 0;
}

int main(void) noexcept {
	HANDLE handle[2];
	handle[0] = CreateThread(nullptr, 0, function_0, nullptr, CREATE_SUSPENDED, nullptr);
	handle[1] = CreateThread(nullptr, 0, function_1, nullptr, CREATE_SUSPENDED, nullptr);
	SetThreadAffinityMask(handle[0], 1 << 0);
	SetThreadAffinityMask(handle[1], 1 << 1);

	system("pause");
	auto rdtsc = __rdtsc();
	ResumeThread(handle[0]);
	ResumeThread(handle[1]);
	WaitForMultipleObjects(1, handle, true, INFINITE);
	std::cout << __rdtsc() - rdtsc << " : " << _value << std::endl;
	system("pause");

	return 0;
}