#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "array.h"
#include <array>
#include <iostream>


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::array<int, 100> std_arrray{ 1,2 };
	library::array<int, 1000> static_array;

	library::array<int, 0> dynamic_array;
	dynamic_array.allocate(10);

	library::array<int, 0> dynamic_array2(dynamic_array);

	return 0;
}