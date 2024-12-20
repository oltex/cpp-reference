#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "unix.h"
#include "date.h"

#include "multimedia.h"
#include "query_performance.h"
#include <iostream>
#include <chrono>

int main(void) noexcept {
	//std::chrono::system_clock::now();
	//std::chrono::steady_clock::now();
	//std::chrono::high_resolution_clock::now();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	time::unix unix;
	unix.time();
	time::date date = unix.local_time();
	std::cout << date.put_time("%y/%m/%d %H:%M:%S") << std::endl;
	return 0;
}