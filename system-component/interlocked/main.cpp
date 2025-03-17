#include "interlocked.h"

int main(void) noexcept {
	unsigned int a;
	system_component::interlocked::increment(a);

	return 0;
}