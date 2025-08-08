#include "storage.h"
#include "../my_class.h"
#include <optional>

int main(void) noexcept {
	std::optional<my_class> a;

	library::storage<my_class> mycls;
	//mycls.construct(10);
	//mycls.get().function();
	//mycls.destruct();


	//my_class* mycls2 = new my_class(20);
	//mycls2->function();
	//delete mycls2;

	//my_class mycls2(20);

	//mycls.memory_copy(mycls2);
	//mycls.get();
	//mycls.destruct();

	//mycls.construct(10);
	//mycls = mycls2;
	//mycls.destruct();
	//mycls.construct(mycls2);
	return 0;
}