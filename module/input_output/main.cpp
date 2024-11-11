#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "completion.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	input_output::completion com(0);
	auto [result, byte, key, overlapped] = com.get_queue_state(0);
	auto [result2, byte2, key2, overlapped2] = com.get_queue_state_tuple(0);

	return 0;
}