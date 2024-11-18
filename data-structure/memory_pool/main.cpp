#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "memory_pool.h"


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	data_structure::memory_pool<my_class> memory_pool;
	my_class& myclass = memory_pool.allocate(1);
	memory_pool.deallocate(myclass);
	my_class& myclass2 = memory_pool.allocate(2);
	memory_pool.deallocate(myclass2);

	return 0;
}