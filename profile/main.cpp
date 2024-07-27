#include <iostream>
#include "profile.h"

int main(void) {
	for (auto i = 0; i < 10; ++i) {
		{
			profiling("B");
			for (auto i = 0; i < 1000000; ++i) {
				int a = 10;
			}
		}
	}
	{
		profiling("BC");
	}

	{
		profiling("ABC");
	}
	profiler::print();
	return 0;
}