#include "engine.h"
#include <iostream>

int main(void) noexcept {
	engine::engine::instance().update();
	return 0;
}