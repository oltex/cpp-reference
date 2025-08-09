#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "array.h"
#include <array>
#include <iostream>


int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::array<int, 100> std_arr{ 1,2 };
	library::array<int, 1000> arr;

	void* test = arr.data();
	return 0;
}