#pragma once
#pragma comment (lib, "winmm")
#include "../../system-component/time/multimedia.h"
#include "../../system-component/time/query_performance.h"
#include "../../data-structure/unordered-map/unordered_map.h"
#include "../../design-pettern/singleton/singleton.h"
#include <Windows.h>
#include <fstream>

//#ifdef dll
//#define declspec_dll _declspec(dllexport)
//#else
//#define declspec_dll _declspec(dllimport)
//#endif

namespace utility {
	class profiler final : public design_pattern::singleton<profiler> {
	private:
		friend class design_pattern::singleton<profiler>;
		using size_type = unsigned int;
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
		inline auto operator=(profiler const&) noexcept -> profiler & = delete;
		inline auto operator=(profiler&&) noexcept -> profiler & = delete;
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
			profile_._pause += timer - profile_._timer;
			profile_._timer.clear();

			double time = static_cast<double>(profile_._pause.data().QuadPart) / system_component::time::_frequency.data().QuadPart;
			if (profile_._minimum > time)
				profile_._minimum = time;
			if (profile_._maximum < time)
				profile_._maximum = time;
			profile_._sum += time;
			++profile_._count;
		}
		inline void pause(char const* const tag) const noexcept {
			system_component::time::query_performance timer;
			timer.counter();
			profile& profile_ = (*_profile.find(tag))._second;
			profile_._pause += timer - profile_._timer;
			profile_._timer.clear();
		}
		inline void resume(char const* const tag) noexcept {
			(*_profile.find(tag))._second._timer.counter();
		}
		inline void clear(char const* const tag) noexcept {
			auto& profiler_ = (*_profile.find(tag))._second;
			profiler_._timer.clear();
			profiler_._pause.clear();
			profiler_._sum = 0;
			profiler_._count = 0;
			profiler_._minimum = DBL_MAX;
			profiler_._maximum = -DBL_MAX;
		}
	public:
		inline void print(void) const noexcept {
			printf("tag | avg | min | max | sum | cnt\n");
			printf("---------------------------\n");
			for (auto& iter : _profile) {
				auto& profiler_ = iter._second;
				printf("%s | %f | %f | %f | %f | %u\n",
					iter._first,
					profiler_._sum / profiler_._count,
					profiler_._minimum,
					profiler_._maximum,
					profiler_._sum,
					profiler_._count);
			}
		}
		inline void export_(char const* const path) const noexcept {
			std::ofstream ofstream(path);
			ofstream << "tag | avg | min | max | sum | cnt\n";
			ofstream << "---------------------------\n";
			for (auto& iter : _profile) {
				auto& profiler_ = iter._second;
				ofstream << iter._first << " | " <<
					profiler_._sum / profiler_._count << " | " <<
					profiler_._minimum << " | " <<
					profiler_._maximum << " | " <<
					profiler_._sum << " | " <<
					profiler_._count << "\n";
			}
			ofstream << "---------------------------\n";
			ofstream.close();
		}
		inline auto result(char const* const tag) noexcept -> profile& {
			return (*_profile.find(tag))._second;
		}
	private:
		data_structure::unordered_map<char const* const, profile> _profile;
	};
}