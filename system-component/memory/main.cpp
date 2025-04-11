#include "memory.h"

int main(void) noexcept {
	auto a = library::system_component::memory::allocate(10);
	library::system_component::memory::deallocate(a);
	return 0;
}