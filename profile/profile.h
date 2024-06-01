#pragma once

#include <Windows.h>
#include <float.h>

#ifdef dll
#define declspec_dll _declspec(dllexport)
#else
#define declspec_dll _declspec(dllimport)
#endif

class profiling_;
class declspec_dll profiler final {
private:
	struct profile {
		char const* tag;
		size_t cnt;
		double sum, avg;
		double min, max;
	};
	friend profiling_;
	explicit profiler(void) noexcept;
	~profiler(void) noexcept;
public:
	static void print(void)  noexcept;
	static void _export(char const* const path)  noexcept;
	static void clear(void)  noexcept {
		for (size_t i = 0; i < instance._size; ++i) {
			instance._arr[i].sum = 0;
			instance._arr[i].cnt = 0;
			instance._arr[i].min = DBL_MAX;
			instance._arr[i].max = -DBL_MAX;
		}
	};
private:
	inline void insert(size_t const idx, char const* const tag, LARGE_INTEGER const* timer) const noexcept {
		_arr[idx].cnt++;
		double cur = static_cast<double>(timer[1].QuadPart - timer[0].QuadPart) / _freq.QuadPart;
		_arr[idx].sum += cur;
		if (_arr[idx].min > cur)
			_arr[idx].min = cur;
		if (_arr[idx].max < cur)
			_arr[idx].max = cur;
	};
	size_t push(char const* const tag) noexcept;
	void reserve(size_t const capacity) noexcept;
private:
	static profiler instance;
	LARGE_INTEGER _freq;
	profile* _arr;
	size_t _size = 0;
	size_t _capacity = 0;
};

class declspec_dll profiling_ final {
public:
	inline explicit profiling_(size_t const idx, char const* const tag) noexcept
		: _idx{ idx }, _tag{ tag } {
		QueryPerformanceCounter(_timer);
	}
	inline ~profiling_(void) noexcept {
		QueryPerformanceCounter(_timer + 1);
		profiler::instance.insert(_idx, _tag, _timer);
	};
public:
	static size_t push(char const* const tag) noexcept;
private:
	size_t const _idx;
	char const* const _tag;
	LARGE_INTEGER _timer[2];
};

#define concat(sour, dest) sour##dest
#define profiling(tag) addline(tag, __LINE__)
#define addline(tag, line) \
static size_t concat(idx, line) = profiling_::push(tag); \
profiling_ concat(_profiling,line) {concat(idx,line), tag};