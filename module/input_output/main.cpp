#include "completion.h"

int main(void) noexcept {
	input_output::completion com(0);


	com.connect_port();
	com.get_queue_state();
	com.post_queue_state();

	return 0;
}