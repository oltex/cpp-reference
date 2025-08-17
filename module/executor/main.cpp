#include "executor.h"

class my_class {
public:
	coroutine function(void) noexcept {
		printf("function");
		co_return;
	}
};

int main(void) noexcept {
	my_class t;
	executor& a = executor::construct(5);
	executor& b = executor::instance();
	//executor.registed(&my_class::function, t);

	Sleep(INFINITE);
}