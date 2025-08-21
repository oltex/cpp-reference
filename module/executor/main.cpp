#include "executor.h"
#include <algorithm>

class my_class {
public:
	coroutine function(void) noexcept {
		printf("function");
		co_return;
	}
};

int main(void) noexcept {
	//my_class t;
	//executor& exec = executor::construct(5);
	//exec.registed(&my_class::function, t);

	Sleep(INFINITE);
}