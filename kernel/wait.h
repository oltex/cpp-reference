#pragma once
#include "object.h"
#include <windows.h>
#include <concepts>

namespace kernel {
	inline static auto wait_for_single_object(HANDLE handle, unsigned long const milli_second) noexcept {
		return WaitForSingleObject(handle, milli_second);
	}
	inline static auto wait_for_multiple_object(unsigned long const count, HANDLE* handle, bool const wait_all, unsigned long const milli_second) noexcept -> unsigned long {
		return WaitForMultipleObjects(count, handle, wait_all, milli_second);
	}
	template<std::derived_from<object>... object_>
	inline static auto wait_for_multiple_object(bool const wait_all, unsigned long const milli_second, object_&... object) noexcept -> unsigned long {
		HANDLE handle[] = { object.data()... };
		return WaitForMultipleObjects(sizeof...(object_), handle, wait_all, milli_second);
	}
}