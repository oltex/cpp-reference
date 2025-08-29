#pragma once
#include "../array.h"
#include "my_class.h"

#include <array>
#include <iostream>

namespace example {
	inline void array(void) noexcept {
		//my_class normal_array[3]{ my_class(1), my_class(2), my_class(3) };
		library::array<my_class, 3> static_array { my_class(1), my_class(2), my_class(3) };

		library::array<my_class, 0> dynamic_array(3, 10);
		//library::array<int, 0> dynamic_array2(dynamic_array);
	}
}