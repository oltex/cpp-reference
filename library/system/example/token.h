#pragma once
#include "../process.h"
#include "../token.h"

namespace example {
	int main(void) noexcept {
		library::process p;
		library::token t = p.open_token();

		t.adjust_privileges_se_lock_memory_privilege();
	}
}