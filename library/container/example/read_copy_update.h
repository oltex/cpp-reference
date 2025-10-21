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
			for (auto loop = 0; loop < 10; ++loop) {
				if (rand() % 10) {
					auto old = value[index];
					value[index] = new int(index);
					//delete cur;
					rcu.retire(old, [](int* pointer) {  delete pointer; });
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
		bool finish;
		int* value;
	};
	inline auto custom_deleter(void* pointer) noexcept {
		delete static_cast<my_str*>(pointer)->value;
		static_cast<my_str*>(pointer)->finish = true;
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
			//for (auto loop = 0; loop < 100; ++loop) {
			if (0 == (rand() % 3)) {
				if (value2[index].invalid == false) {
					value2[index].invalid = true; //���׷��̼��� ������Ŵ => �������� �������͸� �� ���׷��̼����� �����ϰ��ִ��ֵ��� ��ȿ�� : �� �κ����尡�ƴϴ� 
					value2[index].finish = false;
					rcu.retire(&value2[index], custom_deleter);
				}
				else if (value2[index].finish == true) {
					value2[index].value = new int(index);
					value2[index].invalid = false;
				}
			}
			else {
				auto other_index = rand() % count;
				auto pointer = value2[other_index]; //���� ��밡 �ٲٰ� ���� �ôٸ� ���� �����ô� �������� ���׷��̼��� �ٸ������� -> �װ� �������ƴ� 
				if (false == pointer.invalid) {
					auto result = *pointer.value;
					if (other_index != result)
						__debugbreak();
				}
				//else {
				//	auto result = pointer.value;
				//	if (other_index != result)
				//		__debugbreak();
				//}
			}
			//}
			rcu.unlock();
		}
		return 0;
	}

	inline void read_copy_update(void) noexcept {
		scanf_s("%d", &count);
		value = new int* [count];
		for (auto index = 0; index < count; ++index)
			value[index] = new int(index);

		HANDLE* thread = new HANDLE[count];
		for (auto index = 0; index < count; ++index)
			thread[index] = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, read_copy_update_func, nullptr, 0, 0));

		system("pause");
		stop = true;

		WaitForMultipleObjects(count, thread, true, INFINITE);
		for (auto index = 0; index < count; ++index)
			CloseHandle(thread[index]);
		delete[] thread;

		for (auto index = 0; index < count; ++index)
			delete value[index];
		delete[] value;

		//================

		//scanf_s("%d", &count);
		//value2 = new my_str[count];
		//for (auto index = 0; index < count; ++index) {
		//	value2[index].invalid = false;
		//	value2[index].finish = false;
		//	value2[index].value = new int(index);
		//}

		//HANDLE* thread = new HANDLE[count];
		//for (auto index = 0; index < count; ++index)
		//	thread[index] = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, read_copy_update_func2, nullptr, 0, 0));

		//system("pause");
		//stop = true;

		//WaitForMultipleObjects(count, thread, true, INFINITE);
		//for (auto index = 0; index < count; ++index)
		//	CloseHandle(thread[index]);
		//delete[] thread;

		//delete[] value2;
	}
}