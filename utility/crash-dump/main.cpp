#include "crash_dump.h"
#include <iostream>

//void se_translator_function(unsigned int a, EXCEPTION_POINTERS* p) {
//	std::cout << "hello" << std::endl;
//	throw 10;
//}

void terminate_function1(void) noexcept {
	__debugbreak();
}

int main(void) noexcept {
	crash_dump _crash_dump;
	__debugbreak();

	//_set_se_translator(se_translator_function);
	//try {
	//	int a = 0;
	//	int b = 10 / a;
	//	throw 10;
	//}
	//catch (float a) {
	//	int b = 10;
	//}

	//_set_abort_behavior(0, 0xffffffff);
	//abort();
	//std::cout << "hello" << std::endl;

	//std::set_terminate(terminate_function1);
	//terminate();
	//TerminateProcess(GetCurrentProcess(), 0);
	//TerminateThread(GetCurrentThread(), 0);

	return 0;
}