#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "socket.h"
using namespace network;

void func(void) noexcept {
	network::socket s(socket::inet, socket::stream, socket::tcp);
}

void exit(void) noexcept {
	_CrtDumpMemoryLeaks();
}

int main(void) noexcept {
	atexit(exit);
	func();
	return 0;
}