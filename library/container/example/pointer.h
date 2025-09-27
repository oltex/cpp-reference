#pragma once
#include "../pointer.h"
#include "my_class.h"
#include <memory>
#include <iostream>

namespace example {
	//inline void unique_pointer(void) noexcept {
	//	std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
	//	if (arr) {
	//		for (int i = 0; i < 5; ++i)
	//			arr[i] = i * 10;
	//		for (int i = 0; i < 5; ++i)
	//			std::cout << arr[i] << " ";
	//	}

	//	std::unique_ptr<int> std_unique_ptr;
	//	std::unique_ptr<int> std_unique_ptr2(std::move(std_unique_ptr));
	//	std::unique_ptr<int> std_unique_ptr3 = std::move(std_unique_ptr);
	//}
	//inline void share_pointer(void) noexcept {
	//	std::shared_ptr<my_class> std_shard_ptr(nullptr);
	//	std::shared_ptr<my_class> std_shard_ptr2(std_shard_ptr);
	//	std_shard_ptr = nullptr;
	//	//std_shard_ptr.use_count();

	//	library::share_pointer<my_class> share_pointer(10);
	//	library::share_pointer<my_class> share_pointer2(share_pointer);
	//	share_pointer = nullptr;
	//	if (share_pointer == nullptr) {
	//	}
	//	//library::data_structure::shared_pointer<my_class> shared_pointer2(new my_class(2));
	//	//shared_pointer = shared_pointer2;
	//	//my_class* m = shared_pointer.release();
	//	//shared_pointer.swap(shared_pointer2);
	//}
	//inline void weak_pointer(void) noexcept {
	//	std::shared_ptr<int> std_shard_ptr;
	//	std::weak_ptr<int> std_weak_ptr(std_shard_ptr);
	//	std_weak_ptr = std_shard_ptr;
	//	library::share_pointer<int> shard_ptr(1);
	//	library::weak_pointer<int> weak_ptr;
	//	weak_ptr = shard_ptr;
	//}
	inline void my_deleter(derive* pointer) noexcept {
	}

	inline void pointer(void) noexcept {
		{
			//std::shared_ptr<derive> std_pointer(new derive(10, 20));
			//std::shared_ptr<base> std_pointer2 = std_pointer;

			//std::static_pointer_cast<derive>(std_pointer2);
			//int a = 10;
		}
		{
			//library::unique_pointer<derive> pointer(new derive(10, 20));
			//library::unique_pointer<base> pointer2 = pointer;
			library::share_pointer<derive> pointer(new derive(10, 20));
			//library::share_pointer<base> pointer2 = pointer;

			library::weak_pointer<base> pointer3(pointer);
			library::weak_pointer<derive> pointer4(pointer);
			library::weak_pointer<derive> pointer5(pointer4);
			int a = 10;
		}
	}

}