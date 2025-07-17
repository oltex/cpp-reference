#include "pointer.h"
#include <memory>
#include <iostream>

#include "../my_class.h"

int main(void) noexcept {

	std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
	//for (int i = 0; i < 5; ++i) {
	//    arr[i] = i * 10;
	//}
	//for (int i = 0; i < 5; ++i) {
	//    std::cout << arr[i] << " ";
	//}

	//std::unique_ptr<int> std_unique_ptr;
	////std_unique_ptr[10];
	//std::unique_ptr<int> std_unique_ptr2(std::move(std_unique_ptr));
	//std::unique_ptr<int> std_unique_ptr3 = std::move(std_unique_ptr);
	library::unique_pointer<int> a;


	std::shared_ptr<int> std_shard_ptr;
	std::weak_ptr<int> std_weak_ptr(std_shard_ptr);
	std_weak_ptr = std_shard_ptr;
	library::share_pointer<int> shard_ptr(1);
	library::weak_pointer<int> weak_ptr;
	weak_ptr = shard_ptr;
	return 0;
}

#pragma region share_pointer
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#include "shared_pointer.h"
//#include "../my_class.h"
//#include <memory>
//
//int main(void) noexcept {
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	std::shared_ptr<my_class> std_shard_ptr;
//	std::shared_ptr<my_class> std_shard_ptr2(std_shard_ptr);
//	std_shard_ptr = nullptr;
//	//std_shard_ptr.use_count();
//
//	library::data_structure::shared_pointer<my_class> shared_pointer(10);
//	library::data_structure::shared_pointer<my_class> shared_pointer2(shared_pointer);
//	shared_pointer = nullptr;
//	if (shared_pointer == nullptr) {
//
//	}
//	//library::data_structure::shared_pointer<my_class> shared_pointer2(new my_class(2));
//	//shared_pointer = shared_pointer2;
//	//my_class* m = shared_pointer.release();
//	//shared_pointer.swap(shared_pointer2);
//	return 0;
//}
#pragma endregion

#pragma region weak_pointer
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#include "weak_pointer.h"
//#include "../my_class.h"
//#include <memory>
//
//int main(void) noexcept {
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	std::shared_ptr<int> std_shard_ptr;
//	std::weak_ptr<int> std_weak_ptr(std_shard_ptr);
//
//	std_weak_ptr = std_shard_ptr;
//	//template <class _Ty2, enable_if_t<_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
//	//weak_ptr& operator=(const shared_ptr<_Ty2>&_Right) noexcept {
//	//	weak_ptr(_Right).swap(*this);
//	//	return *this;
//	//}
//
//	library::data_structure::shared_pointer<int> shard_ptr(1);
//	library::data_structure::weak_pointer<int> weak_ptr;
//
//	weak_ptr = shard_ptr;
//	return 0;
//}
#pragma endregion
