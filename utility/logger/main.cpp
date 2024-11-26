#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "logger.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	logger& logger = logger::instance();
	logger.set_level(logger::level::info);
	logger.output_console(true);
	logger.output_file(true/*, L"test.txt"*/);

	logger.log(logger::level::info, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	logger.log(logger::level::info, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	logger.log(logger::level::debug, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	logger.log(logger::level::info, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	logger.log(logger::level::info, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	logger.log(logger::level::error, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	logger.log(logger::level::info, "hellowjdfsalkadfslkfdsjladsfkjsdhfkjsdfhjkh");
	logger.log(logger::level::info, "fkerokprepoktrweoptopiwerptiortripotwiopwrtepiow");
	return 0;
}