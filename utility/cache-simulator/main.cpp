#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "cache_simulator.h"
#include <iostream>
#include <string>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto& cache = cache_simulator::instance();

	struct my_struct {
		int a;
		int b;
	};
	my_struct mystr;
	for (auto i = 0; i < 100; ++i) {
		cache_simulator_access(mystr.a);
		cache_simulator_access(mystr.b);
	}

	//cache.access(__FILE__, __LINE__, "a", (void*)0x0fc0);
	//cache.access(__FILE__, __LINE__, "b", (void*)0x1fc0);
	//cache.access(__FILE__, __LINE__, "c", (void*)0x2fc0);
	//cache.access(__FILE__, __LINE__, "d", (void*)0x3fc0);
	//cache.access(__FILE__, __LINE__, "e", (void*)0x4fc0);
	//cache.access(__FILE__, __LINE__, "f", (void*)0x5fc0);
	//cache.access(__FILE__, __LINE__, "g", (void*)0x6fc0);
	//cache.access(__FILE__, __LINE__, "h", (void*)0x7fc0);


	//cache.access(__FILE__, __LINE__, "a", (void*)0x0fc0);
	//cache.access(__FILE__, __LINE__, "b", (void*)0x1fc0);
	//cache.access(__FILE__, __LINE__, "a", (void*)0x0fc0);

	//cache.access(__FILE__, __LINE__, "i", (void*)0x8fc0);

	//cache.access(__FILE__, __LINE__, "e", (void*)0x4fc0);

	cache.print();
	return 0;
}