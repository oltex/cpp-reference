#pragma once
#include "../array.h"

#include <array>
#include <iostream>

namespace example {
	inline void array(void) noexcept {
		std::array<int, 100> std_arrray{ 1,2 };
		library::array<int, 1000> static_array;

		library::array<int, 0> dynamic_array;
		dynamic_array.allocate(10);

		library::array<int, 0> dynamic_array2(dynamic_array);
	}
}