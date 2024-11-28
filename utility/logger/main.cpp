#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "logger.h"

int main(void) noexcept {
	//상대경로로 파일을만듦
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	logger& logger = logger::instance();
	logger.set_level(logger::level::info);
	logger.set_output_console(true);
	logger.set_output_file(true);

	logger.log<logger::output::console | logger::output::file>(logger::level::info, L"key", L"hello : %d", 10);
	logger.log<logger::output::console | logger::output::file>(logger::level::info, L"key", L"bye : %d", 10);


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