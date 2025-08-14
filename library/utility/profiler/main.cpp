#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "profiler.h"
#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto& profiler = utility::profiler::instance();

	profiler.start("C");
	for (int i = 0; i < 100; ++i) {
		profiler.start("B");
		for (auto i = 0; i < 100000; ++i)
			int a = 10;
		profiler.stop("B");
	}
	profiler.stop("C");

	//profiler.clear("B");

	profiler.print();
	profiler.export_("aa.txt");

	return 0;
}