#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "pool.h"


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//library::pool<my_struct, true, true> memory_pool;
	//library::pool<my_struct, true, true> memory_pool2;

	//auto b = memory_pool.allocate();
	//auto b2 = memory_pool.allocate();
	//memory_pool.deallocate(b);
	//memory_pool.deallocate(b2);

	//auto c = memory_pool2.allocate();
	//memory_pool2.deallocate(c);

	return 0;
}