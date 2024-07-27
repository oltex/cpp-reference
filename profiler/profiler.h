#pragma once
#pragma comment (lib, "winmm")

#include "unordered_map.h"

#include <timeapi.h>
#include <profileapi.h>

class profiler final {
private:
	struct profile {
		size_t cnt;
		double sum, avg;
		double min, max;
	};
public:
	inline explicit profiler(void) noexcept {
		//timeBeginPeriod(1);
		QueryPerformanceFrequency(&_freq);
	}
	inline ~profiler(void) noexcept {
		//timeEndPeriod(1);
	}
public:
	void start(char const* tag) noexcept {

	}
	void stop(char const* tag) noexcept {

	}
	void pause(char const* tag) noexcept {

	}
private:
	LARGE_INTEGER _freq;
	unordered_map<char const*, profile> _profile;
};