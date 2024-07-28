#pragma once
#pragma comment (lib, "winmm")

#include "unordered_map.h"
#include "singleton.h"

#include <Windows.h>
#include <fstream>

#ifdef dll
#define declspec_dll _declspec(dllexport)
#else
#define declspec_dll _declspec(dllimport)
#endif

class profiler final : public singleton<profiler> {
	friend class singleton<profiler>;
private:
	struct profile final {
		LARGE_INTEGER _counter;
		LARGE_INTEGER _pause;

		double _sum, _minimum = DBL_MAX, _maximum = -DBL_MAX;
		size_t _count;
	};
private:
	inline explicit profiler(void) noexcept {
		timeBeginPeriod(1);
		QueryPerformanceFrequency(&_frequency);
	}
	inline ~profiler(void) noexcept {
		timeEndPeriod(1);
	}
public:
	inline void start(char const* const tag) noexcept {
		auto& profile = _profile[tag];
		profile._pause.QuadPart = 0;
		QueryPerformanceCounter(&profile._counter);
	}
	inline void stop(char const* const tag) const noexcept {
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		auto iter = _profile.find(tag);
		if (iter == _profile.end())
			return;
		auto& profile = (*iter)._second;

		if (0 == profile._counter.QuadPart)
			return;
		profile._pause.QuadPart += counter.QuadPart - profile._counter.QuadPart;
		profile._counter.QuadPart = 0;

		double time = static_cast<double>(profile._pause.QuadPart) / _frequency.QuadPart;
		profile._pause.QuadPart = 0;

		profile._sum += time;
		if (profile._minimum > time)
			profile._minimum = time;
		if (profile._maximum < time)
			profile._maximum = time;
		++profile._count;
	}
	inline void pause(char const* const tag) const noexcept {
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		auto iter = _profile.find(tag);
		if (iter == _profile.end())
			return;
		auto& profile = (*iter)._second;

		if (0 == profile._counter.QuadPart)
			return;
		profile._pause.QuadPart += counter.QuadPart - profile._counter.QuadPart;
		profile._counter.QuadPart = 0;
	}
	inline void resume(char const* const tag) noexcept {
		QueryPerformanceCounter(&_profile[tag]._counter);
	}
	inline void clear(char const* const tag) noexcept {
		auto& pro = _profile[tag];
		memset(&pro, 0, sizeof(profile));
		pro._minimum = DBL_MAX;
		pro._maximum = -DBL_MAX;
	}
public:
	inline void print(void) const noexcept {
		printf("tag | avg | min | max | sum | cnt\n");
		printf("---------------------------\n");
		for (auto& iter : _profile) {
			auto& pro = iter._second;
			printf("%s | %f | %f | %f | %f | %llu\n",
				iter._first,
				pro._sum / pro._count,
				pro._minimum,
				pro._maximum,
				pro._sum,
				pro._count);
		}
	}
	inline void export_(char const* const path) const noexcept {
		std::ofstream ofs(path);
		ofs << "tag | avg | min | max | sum | cnt\n";
		ofs << "---------------------------\n";
		for (auto& iter : _profile) {
			auto& profile = iter._second;
			ofs << iter._first << " | " <<
				profile._sum / profile._count << " | " <<
				profile._minimum << " | " <<
				profile._maximum << " | " <<
				profile._sum << " | " <<
				profile._count << "\n";
		}
		ofs << "---------------------------\n";
		ofs.close();
	}
private:
	LARGE_INTEGER _frequency;
	unordered_map<char const* const, profile> _profile;
};