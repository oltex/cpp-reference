#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "thread.h"

#include "spin_lock.h"
#include "wait_on_address_lock.h"
#include "critical_section.h"
#include "slim_read_write_lock.h"

#include "ordering.h"

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <conio.h>

std::once_flag flag;

void once(void) noexcept {
	std::cout << "a" << std::endl;
}

void function(void) noexcept {
	__debugbreak();
	std::call_once(flag, once);
	std::cout << "b" << std::endl;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	multi::thread thread1(function, 0);
	multi::thread thread2(function, 0);

	system("pause");
	thread1.resume();
	thread2.resume();

	thread1.join(INFINITE);
	thread2.join(INFINITE);
	system("pause");
	return 0;
}