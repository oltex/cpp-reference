#pragma once
#include <Windows.h>

namespace system_component::time {
	class high_resolution final {
	public:
		inline explicit high_resolution(void) noexcept = default;
		inline explicit high_resolution(high_resolution const& rhs) noexcept
			: _performance_count(rhs._performance_count) {
		};
		inline explicit high_resolution(high_resolution&& rhs) noexcept
			: _performance_count(rhs._performance_count) {
		}
		inline auto operator=(high_resolution const& rhs) noexcept -> high_resolution& {
			_performance_count = rhs._performance_count;
		}
		inline auto operator=(high_resolution&& rhs) noexcept -> high_resolution& {
			_performance_count = rhs._performance_count;
		}
		inline ~high_resolution(void) noexcept = default;
	public:
		inline auto query_performance_counter(void) noexcept -> LARGE_INTEGER {
			QueryPerformanceCounter(&_performance_count);
			return _performance_count;
		}
	public:
		inline auto data(void) noexcept -> LARGE_INTEGER& {
			return _performance_count;
		}
	private:
		LARGE_INTEGER _performance_count;
	};

	inline static auto query_performance_frequency(void) noexcept -> LARGE_INTEGER {
		//LARGE_INTEGER frequency;
		//QueryPerformanceFrequency(&frequency);
		static auto frequency = []() noexcept {
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return frequency;
			}();
			return frequency;
	}
	//inline LARGE_INTEGER query_performance_frequency;
}