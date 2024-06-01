#include <iostream>
#include "profile.h"




int main(void) {
	{
		profiling("A");
		Sleep(1000);
		profiling("B");
		Sleep(500);
		profiling("C");
		Sleep(100);
	}
	for (int i = 0; i < 100; ++i) {
		profiling("D");
		Sleep(10);
	}
	profiler::print();
	profiler::_export("profile.txt");
	return 0;
}