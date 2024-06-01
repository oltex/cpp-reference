#include "profile.h"
#pragma comment (lib, "winmm")
#include <fstream>

profiler profiler::instance;

profiler::profiler(void) noexcept {
	timeBeginPeriod(1);
	QueryPerformanceFrequency(&_freq);
	reserve(4);
}

profiler::~profiler(void) noexcept {
	timeEndPeriod(1);
}

void profiler::print(void)  noexcept {
	printf("tag | avg | min | max | cnt\n");
	printf("---------------------------\n");
	for (size_t i = 0; i < instance._size; ++i) {
		instance._arr[i].avg = instance._arr[i].sum / instance._arr[i].cnt;
		printf("%s | %f | %f | %f | %d\n", 
			instance._arr[i].tag, instance._arr[i].avg, 
			instance._arr[i].min, instance._arr[i].max, 
			instance._arr[i].cnt);
	}
}

void profiler::_export(char const* const path)  noexcept {
	std::ofstream ofs(path);
	ofs << "tag | avg | min | max | cnt\n";
	ofs << "---------------------------\n";
	for (size_t i = 0; i < instance._size; ++i) {
		instance._arr[i].avg = instance._arr[i].sum / instance._arr[i].cnt;
		ofs << instance._arr[i].tag << " | " << instance._arr[i].avg << " | " 
			<< instance._arr[i].min << " | " << instance._arr[i].max << " | "
			<< instance._arr[i].cnt << "\n";
	}
	ofs << "---------------------------\n";
	ofs.close();
}