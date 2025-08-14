#pragma once
#include "../weak_pointer.h"
#include "my_class.h"

#include <memory>
#include <iostream>

namespace example {
	inline void weak_pointer(void) noexcept {
		std::shared_ptr<int> std_shard_ptr;
		std::weak_ptr<int> std_weak_ptr(std_shard_ptr);
		std_weak_ptr = std_shard_ptr;
		library::share_pointer<int> shard_ptr(1);
		library::weak_pointer<int> weak_ptr;
		weak_ptr = shard_ptr;
	}
}