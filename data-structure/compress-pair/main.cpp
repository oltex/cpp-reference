#include "compress_pair.h"
#include <xmemory>
#include <iostream>
struct empty {
	void func(void) noexcept {
		std::cout << "hello" << std::endl;
	}
};


struct test : private empty {
	int a;
};

int main(void) noexcept {
	//std::_Compressed_pair<empty, int> std_compress_pair(std::_Zero_then_variadic_args_t{}, 10);
	library::compress_pair<empty, int> compress_pair;
	compress_pair.first().func();
	return 0;
}