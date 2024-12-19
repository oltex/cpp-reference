#pragma once
#pragma comment (lib, "winmm")
#include "../../system-component/time/multimedia.h"
#include "../../system-component/time/high_resolution.h"
#include "../../data-structure/unordered-map/unordered_map.h"
#include "../../design-pettern/singleton/singleton.h"

#include <Windows.h>
#include <fstream>
#include <chrono>

//#ifdef dll
//#define declspec_dll _declspec(dllexport)
//#else
//#define declspec_dll _declspec(dllimport)
//#endif

namespace utility {
	class profiler final : public design_pattern::singleton<profiler> {
	private:
		friend class design_pattern::singleton<profiler>;
		struct profile final {
			LARGE_INTEGER _counter;
			LARGE_INTEGER _pause;

			double _sum, _minimum = DBL_MAX, _maximum = -DBL_MAX;
			size_t _count;
		};
	private:
		inline explicit profiler(void) noexcept {
			timer::multimedia::begin_period(1);
			QueryPerformanceFrequency(&_frequency);
		}
		inline explicit profiler(profiler const& rhs) noexcept = delete;
		inline explicit profiler(profiler&& rhs) noexcept = delete;
		inline auto operator=(profiler const& rhs) noexcept -> singleton & = delete;
		inline auto operator=(profiler&& rhs) noexcept -> singleton & = delete;
		inline ~profiler(void) noexcept {
			timer::multimedia::end_period(1);
		}
	public:
		inline void start(char const* const tag) noexcept {
			auto& prof = _profile[tag];
			prof._pause.QuadPart = 0;
			QueryPerformanceCounter(&prof._counter);
		}
		inline void stop(char const* const tag) const noexcept {
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);

			auto iter = _profile.find(tag);
			if (iter == _profile.end())
				return;
			auto& prof = (*iter)._second;

			if (0 == prof._counter.QuadPart)
				return;
			prof._pause.QuadPart += counter.QuadPart - prof._counter.QuadPart;
			prof._counter.QuadPart = 0;

			double time = static_cast<double>(prof._pause.QuadPart) / _frequency.QuadPart;
			prof._pause.QuadPart = 0;

			prof._sum += time;
			if (prof._minimum > time)
				prof._minimum = time;
			if (prof._maximum < time)
				prof._maximum = time;
			++prof._count;
		}
		inline void pause(char const* const tag) const noexcept {
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);

			auto iter = _profile.find(tag);
			if (iter == _profile.end())
				return;
			auto& prof = (*iter)._second;

			if (0 == prof._counter.QuadPart)
				return;
			prof._pause.QuadPart += counter.QuadPart - prof._counter.QuadPart;
			prof._counter.QuadPart = 0;
		}
		inline void resume(char const* const tag) noexcept {
			QueryPerformanceCounter(&_profile[tag]._counter);
		}
		inline void clear(char const* const tag) noexcept {
			auto& prof = _profile[tag];
			memset(&prof, 0, sizeof(profile));
			prof._minimum = DBL_MAX;
			prof._maximum = -DBL_MAX;
		}
	public:
		inline void print(void) const noexcept {
			printf("tag | avg | min | max | sum | cnt\n");
			printf("---------------------------\n");
			for (auto& iter : _profile) {
				auto& prof = iter._second;
				printf("%s | %f | %f | %f | %f | %llu\n",
					iter._first,
					prof._sum / prof._count,
					prof._minimum,
					prof._maximum,
					prof._sum,
					prof._count);
			}
		}
		inline void export_(char const* const path) const noexcept {
			std::ofstream ofs(path);
			ofs << "tag | avg | min | max | sum | cnt\n";
			ofs << "---------------------------\n";
			for (auto& iter : _profile) {
				auto& prof = iter._second;
				ofs << iter._first << " | " <<
					prof._sum / prof._count << " | " <<
					prof._minimum << " | " <<
					prof._maximum << " | " <<
					prof._sum << " | " <<
					prof._count << "\n";
			}
			ofs << "---------------------------\n";
			ofs.close();
		}
		inline auto result(char const* const tag)  noexcept -> profile& {
			auto iter = _profile.find(tag);
			return (*iter)._second;
		}
	private:
		LARGE_INTEGER _frequency;
		thread_local void* p; k
			data_structure::unordered_map<char const* const, profile> _profile;
	};
}
