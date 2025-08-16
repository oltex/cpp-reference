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

	executor executor(5);
	executor.registed(&my_class::function, t);

	Sleep(INFINITE);
}