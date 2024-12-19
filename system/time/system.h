#pragma once
#include <Windows.h>
namespace timer::system {
	inline static auto get_tick_count(void) noexcept -> unsigned long {
#pragma warning(suppress: 28159)
		return GetTickCount();
	}
	inline static auto get_tick_count_64(void) noexcept -> unsigned long long {
		return GetTickCount64();
	}
}