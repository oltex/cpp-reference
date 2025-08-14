#pragma once
#include "../share_pointer.h"
#include "my_class.h"

#include <memory>
#include <iostream>

namespace example {
	inline void share_pointer(void) noexcept {
		std::shared_ptr<my_class> std_shard_ptr(nullptr);
		std::shared_ptr<my_class> std_shard_ptr2(std_shard_ptr);
		std_shard_ptr = nullptr;
		//std_shard_ptr.use_count();

		library::share_pointer<my_class> share_pointer(10);
		library::share_pointer<my_class> share_pointer2(share_pointer);
		share_pointer = nullptr;
		if (share_pointer == nullptr) {
		}
		//library::data_structure::shared_pointer<my_class> shared_pointer2(new my_class(2));
		//shared_pointer = shared_pointer2;
		//my_class* m = shared_pointer.release();
		//shared_pointer.swap(shared_pointer2);
	}
}