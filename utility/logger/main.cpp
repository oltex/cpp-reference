#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "logger.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	logger& logger = logger::instance();
	logger.set_level(logger::level::info);

	struct MyStruct {
		int a = 1;
		int b = 0xffffffff;
		int c = 3;
	};
	MyStruct mystr;

	logger.log_message<logger::output::console, logger::level::info>(L"", L"bye : %d", 10);
	logger.log_memory<logger::output::console, logger::level::info>(L"", L"data : ", (unsigned char*)&mystr, sizeof(MyStruct), 4);
	return 0;
}