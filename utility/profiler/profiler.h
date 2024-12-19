#pragma once
#pragma comment (lib, "winmm")
#include "../../system-component/time/multimedia.h"
#include "../../system-component/time/query_performance.h"
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
		using size_type = unsigned int;
	private:
		friend class design_pattern::singleton<profiler>;
		struct profile final {
			system_component::time::query_performance _timer;
			system_component::time::query_performance _pause;
			double _sum, _minimum = DBL_MAX, _maximum = -DBL_MAX;
			size_type _count;
		};
	private:
		inline explicit profiler(void) noexcept {
			system_component::time::multimedia::begin_period(1);
		}
		inline explicit profiler(profiler const&) noexcept = delete;
		inline explicit profiler(profiler&&) noexcept = delete;
		inline auto operator=(profiler const&) noexcept -> singleton & = delete;
		inline auto operator=(profiler&&) noexcept -> singleton & = delete;
		inline ~profiler(void) noexcept {
			system_component::time::multimedia::end_period(1);
		}
	public:
		inline void start(char const* const tag) noexcept {
			profile& profile_ = _profile[tag];
			profile_._pause.clear();
			profile_._timer.counter();
		}
		inline void stop(char const* const tag) const noexcept {
			system_component::time::query_performance timer;
			timer.counter();

			profile& profile_ = (*_profile.find(tag))._second;
			double time = static_cast<double>(timer.data().QuadPart - profile_._timer.data().QuadPart) / system_component::time::_frequency.data().QuadPart;
			profile_._sum += time;
			if (profile_._minimum > time)
				profile_._minimum = time;
			if (profile_._maximum < time)
				profile_._maximum = time;
			profile_._timer.clear();
			++profile_._count;
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
		inline static thread_local void* thiread = 0;
		data_structure::unordered_map<char const* const, profile> _profile;
	};
}
