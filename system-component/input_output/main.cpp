#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "completion_port.h"

int main(void) noexcept {

	system_component::input_output::completion_port com(0);
	auto [result, byte, key, overlapped] = com.get_queue_state(0);
	auto [result2, byte2, key2, overlapped2] = com.get_queue_state_tuple(0);

	return 0;
}