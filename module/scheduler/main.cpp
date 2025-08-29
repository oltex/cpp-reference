#include "io_complet_port.h"
#include "thread_pool.h"

int function(void) noexcept {
	printf("hello\n");
	return 500;
}

int main(void) noexcept {
	auto& instance = thread_pool::construct(4);
	system("pause");
	instance.execute(function);
	system("pause");
}