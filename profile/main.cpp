#include <iostream>
#include "profile.h"

int main(void) {
	{
		profiling("B");
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