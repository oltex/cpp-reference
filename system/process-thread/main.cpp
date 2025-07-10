#include "process_thread.h"

int main(void) noexcept {
	library::system::process p;
	library::system::token t = p.open_token();

	t.adjust_privileges_se_lock_memory_privilege();
}