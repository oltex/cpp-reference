#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "logger_singleton.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	utility::logger& logger = utility::logger::instance();
	logger.set_level(utility::logger::level::info);
	logger.set_output(utility::logger::output::file | utility::logger::output::console);
	logger.create("test", L"test\\test.log");

	//struct MyStruct {
	//	int a = 1;
	//	int b = 0xffffffff;
	//	int c = 3;
	//};
	//MyStruct mystr;

	//logger.log_message<logger::level::info>("test", L"bye : %d", 10);
	log_message(utility::logger::level::info, "test", L"bye : %d", 10);
	//logger.log_memory<logger::output::console, logger::level::info>(L"", L"data : ", (unsigned char*)&mystr, sizeof(MyStruct), 4);
	return 0;
}