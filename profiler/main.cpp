#include "profiler.h"
#include <iostream>

//������ �������ϸ��� �±׸� �������� ���ƾ��Ѵ�.

int main(void) {
	auto& profiler = profiler::instance();

	profiler.start("B");
	for (auto i = 0; i < 1000000; ++i)
		int a = 10;
	profiler.stop("B");
	profiler.clear("B");

	profiler.print();
	profiler.export_("aa.txt");
	return 0;
}