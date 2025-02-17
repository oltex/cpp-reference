#include "coroutine.h"


system_component::coroutine test(void) noexcept {
	printf("test start\n");
	int res = co_await 10;// std::suspend_always{};

	printf("test end\n");

	co_return;
}

int main(void) noexcept {
	auto co = test();
	printf("main start\n");
	co.resume();
	printf("main end\n");
	//co.resume();
	//printf("main end2\n");


	return 0;
}