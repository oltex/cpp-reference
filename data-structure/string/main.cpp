#include "string.h"
#include <string>
#include <iostream>
int main(void) noexcept {
	std::string std_string;
	//std_string.at()
	//std_string.insert()
	//std_string.push_back()
	//std_string = "hello";
	library::string string;
	//string.reserve(31);
	string.push_back('1');
	string.push_back('2');
	string.push_back('3');
	string.push_back('4');
	string.push_back('5');
	string.push_back('6');
	string = "hello";
	string += "guy";
	auto iter = string.begin();
	string.insert(iter, "hello");

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