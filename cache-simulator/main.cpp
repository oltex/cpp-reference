#include "cache_simulator.h"
#include <iostream>

int main() {
	auto& cache = cache_simulator::instance();
	cache.access((void*)0xfc0);
	cache.access((void*)0x1fc0);
	cache.access((void*)0x2fc0);
	cache.access((void*)0x3fc0);
	cache.access((void*)0x4fc0);
	cache.access((void*)0x5fc0);
	cache.access((void*)0x6fc0);
	cache.access((void*)0x7fc0);


	cache.access((void*)0xfc0);
	cache.access((void*)0x1fc0);
	cache.access((void*)0xfc0);

	cache.access((void*)0x8fc0);

	cache.access((void*)0x4fc0);

	return 0;
}