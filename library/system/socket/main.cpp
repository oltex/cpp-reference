#include "socket.h"

int main(void) noexcept {
	library::socket s;
	library::socket s2(std::move(s));

	
	return 0;
}