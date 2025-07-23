#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "weak_pointer.h"
#include "../my_class.h"

#include <memory>
#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::shared_ptr<int> std_shard_ptr;
	std::weak_ptr<int> std_weak_ptr(std_shard_ptr);
	std_weak_ptr = std_shard_ptr;
	library::share_pointer<int> shard_ptr(1);
	library::weak_pointer<int> weak_ptr;
	weak_ptr = shard_ptr;

	return 0;
}