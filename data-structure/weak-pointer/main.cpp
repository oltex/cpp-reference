#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "weak_pointer.h"
#include "../my_class.h"
#include <memory>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::shared_ptr<int> std_shard_ptr;
	std::weak_ptr<int> std_weak_ptr(std_shard_ptr);

	std_weak_ptr = std_shard_ptr;
	//template <class _Ty2, enable_if_t<_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
	//weak_ptr& operator=(const shared_ptr<_Ty2>&_Right) noexcept {
	//	weak_ptr(_Right).swap(*this);
	//	return *this;
	//}

	data_structure::shared_pointer<int> shard_ptr(1);
	data_structure::weak_pointer<int> weak_ptr;

	weak_ptr = shard_ptr;
	return 0;
}