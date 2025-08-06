#include "inputoutput_completion_port.h"

int main(void) noexcept {
	library::system::inputoutput_completion_port iocp;
	auto a = iocp.get_queue_state(INFINITE);
	return 0;
}