#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "shared_pointer.h"
#include "../my_class.h"
#include <memory>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::shared_ptr<my_class> std_shard_ptr;
	std::shared_ptr<my_class> std_shard_ptr2(std_shard_ptr);

	std_shard_ptr = nullptr;
	//std_shard_ptr.use_count();
	library::data_structure::shared_pointer<my_class> shared_pointer;
	library::data_structure::shared_pointer<my_class> shared_pointer2(shared_pointer);
	shared_pointer = nullptr;
	if (shared_pointer == nullptr) {

	}
	//library::data_structure::shared_pointer<my_class> shared_pointer2(new my_class(2));
	//shared_pointer = shared_pointer2;
	//my_class* m = shared_pointer.release();
	//shared_pointer.swap(shared_pointer2);
	return 0;
}
