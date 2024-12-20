#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "thread.h"

#include "lock/spin.h"
#include "lock/wait_on_address.h"
#include "lock/critical_section.h"
#include "lock/slim_read_write.h"

#include "ordering.h"

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <conio.h>


void function(int a) noexcept {
	__debugbreak();
	std::cout << "b" << std::endl;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int a = 10;
	std::thread thread1(function, a);
	std::thread thread2(function, a);
	system("pause");
	thread1.join();
	thread2.join();
	system("pause");
	return 0;
}