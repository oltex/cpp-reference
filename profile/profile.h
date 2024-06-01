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
	static inline size_t push(char const* const tag) noexcept {
		if (instance._size + 1 > instance._capacity)
			instance.reserve(static_cast<int>(instance._capacity * 1.5f));
		instance._arr[instance._size].tag = tag;
		instance._arr[instance._size].min = DBL_MAX;
		instance._arr[instance._size].max = -DBL_MAX;
		return instance._size++;
	}
	inline void clear(void) const noexcept {
		for (size_t i = 0; i < _size; ++i) {
			_arr[i].sum = 0;
			_arr[i].cnt = 0;
			_arr[i].min = DBL_MAX;
			_arr[i].max = -DBL_MAX;
		}
	};
private:
	static inline void insert(size_t const idx, char const* const tag, LARGE_INTEGER const* timer)  noexcept {
		instance._arr[idx].cnt++;
		double cur = static_cast<double>(timer[1].QuadPart - timer[0].QuadPart) / instance._freq.QuadPart;
		instance._arr[idx].sum += cur;
		if (instance._arr[idx].min > cur)
			instance._arr[idx].min = cur;
		if (instance._arr[idx].max < cur)
			instance._arr[idx].max = cur;
	};
	inline void reserve(size_t const capacity) noexcept {
		_capacity = capacity;
		profile* arr = _arr;
		_arr = new profile[capacity];
		memset(_arr, 0, sizeof(profile) * _capacity);
		memcpy(_arr, arr, sizeof(profile) * _size < capacity ? _size : capacity);
		delete[] arr;
	}
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
		profiler::insert(_idx, _tag, _timer);
	};
private:
	size_t const _idx;
	char const* const _tag;
	LARGE_INTEGER _timer[2];
};

#define concat(sour, dest) sour##dest
#define profiling(tag) profiling_line(tag, __LINE__)
#define profiling_line(tag, line) \
static size_t concat(idx, line) = profiler::push(tag); \
profiling_ concat(_profiling,line) {concat(idx,line), tag};
#undef profiling1




