#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "performance_data_help.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto& pdh = library::pdh_query::instance();
	auto& counter = pdh.add_counter(L"\\Processor(_Total)\\% Processor Time");
	pdh.add_counter(L"\\Processor(_Total)\\% Processor Time");

	pdh.collect_query_data();
	auto t = counter.get_format_value<double>();
	return 0;
}