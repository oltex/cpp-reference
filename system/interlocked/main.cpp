#include "interlocked.h"

int main(void) noexcept {
	unsigned int a;
	library::system::interlocked::increment(a);

	return 0;
}