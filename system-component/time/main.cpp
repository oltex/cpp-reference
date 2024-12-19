#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "multimedia.h"
#include "high_resolution.h"
#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	library::module::timer::high_resolution::query_performance_frequency();
	return 0;
}