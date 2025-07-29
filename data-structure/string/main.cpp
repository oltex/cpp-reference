#include "string.h"
#include <string>
#include <iostream>
int main(void) noexcept {
	std::string std_string;
	library::string string;

	size_t capacity = 0;
	for (auto i = 0; i < 10000; ++i) {
		size_t new_capacity = std_string.capacity();
		if (capacity != new_capacity) {
			std::cout << new_capacity << std::endl;
			capacity = new_capacity;
		}
		std_string += 'a';

	}

	return 0;
}