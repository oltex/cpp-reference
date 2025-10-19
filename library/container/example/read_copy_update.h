#pragma once
#include "../read_copy_update.h"
#include "../../debug.h"
#include <thread>





namespace example {

	bool stop = false;
	int static_index = 0;
	int count;
	int** value;
	inline static unsigned int __stdcall read_copy_update_func(void* arg) noexcept {
		library::read_copy_update& rcu = library::read_copy_update::instance();
		auto index = library::interlock_increment(static_index) - 1;
		auto debug = 0;
		while (!stop) {
			if (debug++ == 10000) {
				printf("thread : %d\n", index);
				debug = 0;
			}
			rcu.lock();
			for (auto loop = 0; loop < 100; ++loop) {
				if (rand() % 2) {
					auto old = value[index];
					value[index] = new int(index);
					//delete cur;
					rcu.retire(old);
				}
				else {
					auto other_index = rand() % count;
					auto pointer = value[other_index];
					auto result = *pointer;
					if (other_index != result)
						__debugbreak();
				}
			}
			rcu.unlock();
		}
		return 0;
	}

	struct my_str {
		bool invalid;
		int value;
	};
	inline auto custom_deleter(void* pointer) noexcept {
		static_cast<my_str*>(pointer)->value = 0x11223344; //뭘 망가트리든 상관없음 
	}
	my_str* value2;
	inline static unsigned int __stdcall read_copy_update_func2(void* arg) noexcept {
		library::read_copy_update& rcu = library::read_copy_update::instance();
		auto index = library::interlock_increment(static_index) - 1;
		auto debug = 0;
		while (!stop) {
			if (debug++ == 10000) {
				printf("thread : %d\n", index);
				debug = 0;
			}
			rcu.lock();
			for (auto loop = 0; loop < 100; ++loop) {
				if (rand() % 2) {
					if (value2[index].value != 0x11223344) {
						value2[index].invalid = true; //제네레이션을 증가시킴 => 이제부터 이포인터를 전 제네레이션으로 참조하고있던애들은 무효다 : 즉 인벨리드가아니다 
						//delete cur;
						rcu.retire(&value2[index], custom_deleter);
					}
					else {
						value2[index].value = index;
						value2[index].invalid = false;
					}
				}
				else {
					auto other_index = rand() % count;
					auto pointer = value2[other_index]; //만약 상대가 바꾸고 나서 봤다면 내가 에폭시는 빠르지만 제네레이션이 다를수있음 -> 그건 문제가아님 
					if (false == pointer.invalid) {
						auto result = pointer.value;
						if (other_index != result)
							__debugbreak();
					}
					//else {
					//	auto result = pointer.value;
					//	if (other_index != result)
					//		__debugbreak();
					//}
				}
			}
			rcu.unlock();
		}
		return 0;
	}

	inline void read_copy_update(void) noexcept {
		//scanf_s("%d", &count);
		//value = new int* [count];
		//for (auto index = 0; index < count; ++index)
		//	value[index] = new int(index);

		//HANDLE* thread = new HANDLE[count];
		//for (auto index = 0; index < count; ++index)
		//	thread[index] = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, read_copy_update_func, nullptr, 0, 0));

		//system("pause");
		//stop = true;

		//WaitForMultipleObjects(count, thread, true, INFINITE);
		//for (auto index = 0; index < count; ++index)
		//	CloseHandle(thread[index]);
		//delete[] thread;

		//for (auto index = 0; index < count; ++index)
		//	delete value[index];
		//delete[] value;

		//================

		scanf_s("%d", &count);
		value2 = new my_str[count];
		for (auto index = 0; index < count; ++index) {
			value2[index].invalid = false;
			value2[index].value = index;
		}

		HANDLE* thread = new HANDLE[count];
		for (auto index = 0; index < count; ++index)
			thread[index] = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, read_copy_update_func2, nullptr, 0, 0));

		system("pause");
		stop = true;

		WaitForMultipleObjects(count, thread, true, INFINITE);
		for (auto index = 0; index < count; ++index)
			CloseHandle(thread[index]);
		delete[] thread;

		delete[] value2;
	}
}