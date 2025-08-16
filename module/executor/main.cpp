#include "executor.h"

class test {
public:
	moduler::coroutine<moduler::promise> func(void) noexcept {
		printf("func");
		co_return;
	}
};

int main(void) noexcept {
	test t;

	moduler::executor executor(5);
	executor.registed(&test::func, t);

	Sleep(INFINITE);
}