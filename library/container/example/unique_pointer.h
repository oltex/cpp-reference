#pragma once
#include "../unique_pointer.h"
#include "my_class.h"

#include <memory>
#include <iostream>

namespace example {
	inline void unique_pointer(void) noexcept {
		std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
		if (arr) {
			for (int i = 0; i < 5; ++i)
				arr[i] = i * 10;
			for (int i = 0; i < 5; ++i)
				std::cout << arr[i] << " ";
		}

		std::unique_ptr<int> std_unique_ptr;
		std::unique_ptr<int> std_unique_ptr2(std::move(std_unique_ptr));
		std::unique_ptr<int> std_unique_ptr3 = std::move(std_unique_ptr);
	}
}