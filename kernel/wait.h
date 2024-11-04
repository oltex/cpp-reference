#pragma once
#include "object.h"
#include <windows.h>
#include <concepts>

namespace kernel {
	inline static auto wait_for_single_object(HANDLE object, unsigned long const milli_second) noexcept {
		return WaitForSingleObject(object, milli_second);
	}
	template<std::derived_from<object>... handle>
	inline static auto wait_for_multiple_obejct(bool const wait_all, unsigned long const milli_second, handle... object) noexcept -> unsigned long {
		HANDLE objects[] = { object.data()...};
		return WaitForMultipleObjects(sizeof...(handle), objects, wait_all, milli_second);
	}
}