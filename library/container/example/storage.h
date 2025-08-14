#pragma once
#include "../storage.h"
#include "my_class.h"

namespace example {
	inline void stroage(void) noexcept {
		library::storage<my_class> mycls;
		my_class mycls2(20);

		mycls.relocate(mycls2);
		mycls.destruct();

		mycls.construct(20);
		mycls->function();
		mycls.destruct();


		//mycls.memory_copy(mycls2);
		//mycls.get();
		//mycls.destruct();

		//mycls.construct(10);
		//mycls = mycls2;
		//mycls.destruct();
		//mycls.construct(mycls2);
	}
}