#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "pool.h"

struct my_struct {
	int a;
};
struct my_struct2 {
	int a;
};

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	library::pool<my_struct, true, true> memory_pool;
	library::pool<my_struct, true, true> memory_pool2;

	auto& b = memory_pool.allocate();
	memory_pool.deallocate(b);

	auto& c = memory_pool2.allocate();
	memory_pool2.deallocate(c);
	return 0;
}