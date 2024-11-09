#pragma once
#include <Windows.h>

namespace timer {
	namespace high_resolution {
		inline static auto query_performance_counter(void) noexcept -> LARGE_INTEGER {
			LARGE_INTEGER performance_count;
			QueryPerformanceCounter(&performance_count);
			return performance_count;
		}
		inline static auto query_performance_frequency(void) noexcept -> LARGE_INTEGER {
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return frequency;
		}
	};
}