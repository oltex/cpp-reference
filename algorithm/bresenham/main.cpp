#include "bresenham.h"
#include <iostream>

int main(void) {
	bresenham bresenham;

	for (;;) {
		system("cls");
		auto result = bresenham.calculate(bresenham::coordinate{ (unsigned int)rand() % 100, (unsigned int)rand() % 100 },
			bresenham::coordinate{ (unsigned int)rand() % 100, (unsigned int)rand() % 100 });
		for (auto& iter : result)
			std::cout << iter._x << ", " << iter._y << std::endl;
	}
	return 0;
}