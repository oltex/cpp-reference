#include "executor.h"
#include "lock.h"
#include <algorithm>

class my_class {
public:
	coroutine function(void) noexcept {
		printf("function");
		co_return;
	}
};

int main(void) noexcept {
	push_lock pushlock;
	pushlock.acquire_share();
	pushlock.acquire_share();
	pushlock.acquire_share();

	pushlock.acquire_exclusive();
	pushlock.acquire_exclusive();
	pushlock.acquire_exclusive();
	pushlock.acquire_exclusive();
	//pushlock.acquire_exclusive();

	pushlock.release_exclusive();
	//my_class t;
	//executor& exec = executor::construct(5);
	//exec.registed(&my_class::function, t);

	Sleep(INFINITE);
}