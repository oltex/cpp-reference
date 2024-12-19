#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "unix_time.h"
#include "time_struct.h"

#include "multimedia.h"
#include "high_resolution.h"
#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	system_component::time::unix_time unix_time;
	unix_time.time();
	system_component::time::time_struct time_struct = unix_time.local_time();
	std::cout << time_struct.put_time("%y/%m/%d %H:%M:%S") << std::endl;
	return 0;
}