#include "push_lock.h"

int main(void) noexcept {
	push_lock pl;

	pl.acquire_exclusive();
	pl.acquire_share();
	pl.release_exclusive();
	pl.acquire_share();
	pl.acquire_exclusive();
	pl.acquire_share();
	pl.acquire_share();

	return 0;
}