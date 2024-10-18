#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "thread.h"
#include "spin_lock.h"
#include <thread>

int function(int arg) {
	return arg;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	thread::spin_lock spin_lock;
	spin_lock.lock();
	spin_lock.lock();

	std::thread std_thread;
	thread::thread thread(function, );

	return 0;
}