#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "unix.h"
#include "date.h"

#include "multimedia.h"
#include "query_performance.h"
#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	system_component::time::unix unix;
	unix.time();
	system_component::time::date date = unix.local_time();
	std::cout << date.put_time("%y/%m/%d %H:%M:%S") << std::endl;
	return 0;
}