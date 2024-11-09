//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "allocator.h"
#include <iostream>
#include <list>
#include <intrin.h>

struct my_struct {
	char a[7];
};

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	data_structure::allocator<my_struct> allocator;
	//allocator.allocate(4);

	return 0;
}