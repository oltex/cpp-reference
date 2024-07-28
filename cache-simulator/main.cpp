#include "cache_simulator.h"

#include <iostream>

//#define STRINGIFY(x) #x
//#define TOSTRING(x) STRINGIFY(x)
//
//
//#include <iostream>
//
//#define STRINGIFY(x) #x
//#define TOSTRING(x) STRINGIFY(x)
//
//int main() {
//	int myVariable = 42;
//	std::cout << "The name of the variable is: " << TOSTRING(myVariable) << std::endl;
//	std::cout << "The value of the variable is: " << myVariable << std::endl;
//	return 0;
//}

int main() noexcept {
	auto& cache = cache_simulator::instance();
	cache.access((void*)0x0fc0);
	cache.access((void*)0x1fc0);
	cache.access((void*)0x2fc0);
	cache.access((void*)0x3fc0);
	cache.access((void*)0x4fc0);
	cache.access((void*)0x5fc0);
	cache.access((void*)0x6fc0);
	cache.access((void*)0x7fc0);


	cache.access((void*)0x0fc0);
	cache.access((void*)0x1fc0);
	cache.access((void*)0x0fc0);

	cache.access((void*)0x8fc0);

	cache.access((void*)0x3fc0);
	return 0;
}