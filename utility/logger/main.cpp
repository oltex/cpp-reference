#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "logger.h"

int main(void) noexcept {
	//상대경로로 파일을만듦
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	logger& logger = logger::instance();
	logger.set_level(logger::level::info);
	logger.set_output(logger::console | logger::file);
	logger.set_column(4);

	struct MyStruct {
		int a = 1;
		int b = 0xffffffff;
		int c = 3;
	};

	MyStruct mystr;

	logger.log_message(logger::output::console, logger::level::info, L"", L"bye : %d", 10);

	logger.log_memory(logger::output::console, logger::level::info, L"", L"data : ", (unsigned char*)&mystr, sizeof(MyStruct));




	//logger.log(logger::level::info, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	//logger.log(logger::level::info, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	//logger.log(logger::level::debug, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	//logger.log(logger::level::info, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	//logger.log(logger::level::info, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	//logger.log(logger::level::error, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	//logger.log(logger::level::info, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	//logger.log(logger::level::info, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	return 0;
}