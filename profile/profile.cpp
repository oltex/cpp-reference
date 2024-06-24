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
	delete[] _arr;
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

void profiler::export_(char const* const path)  noexcept {
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

void profiler::clear(void) noexcept {
	for (size_t i = 0; i < instance._size; ++i) {
		instance._arr[i].sum = 0;
		instance._arr[i].cnt = 0;
		instance._arr[i].min = DBL_MAX;
		instance._arr[i].max = -DBL_MAX;
	}
};

size_t profiler::push(char const* const tag) noexcept {
	if (_size + 1 > _capacity)
		instance.reserve(static_cast<int>(_capacity * 1.5f));
	_arr[instance._size].tag = tag;
	_arr[instance._size].min = DBL_MAX;
	_arr[instance._size].max = -DBL_MAX;
	return instance._size++;
}

void profiler::reserve(size_t const capacity) noexcept {
	_capacity = capacity;
	profile* arr = _arr;
	_arr = new profile[capacity];
	memset(_arr, 0, sizeof(profile) * _capacity);
	memcpy(_arr, arr, sizeof(profile) * _size < capacity ? _size : capacity);
	delete[] arr;
}

size_t profiling_::push(char const* const tag) noexcept {
	return profiler::instance.push(tag);
}