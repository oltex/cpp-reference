#pragma once
#include "../read_copy_update.h"
#include "../../debug.h"
#include <thread>

bool stop = false;

int static_index = 0;
int count;
int** value;

namespace example {
	inline static unsigned int __stdcall read_copy_update_func(void* arg) noexcept {
		library::read_copy_update& rcu = *reinterpret_cast<library::read_copy_update*>(arg);
		auto index = library::interlock_increment(static_index) - 1;
		auto zzko = 0;
		while (!stop) {
			if (zzko++ == 10000) {
				printf("thread : %d\n", index);
				zzko = 0;
			}
			rcu.lock();
			for (auto loop = 0; loop < 100; ++loop) {
				if (rand() % 2) {
					auto cur = value[index];
					value[index] = new int(index);
					//delete cur;
					rcu.retire(cur);
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

	inline void read_copy_update(void) noexcept {
		//auto a = new int(0);
		//auto b = a;
		//delete a;
		//printf("%d", *b);
		//return;
		library::read_copy_update rcu;
		scanf_s("%d", &count);
		value = new int* [count];
		for (auto index = 0; index < count; ++index)
			value[index] = new int(index);

		HANDLE* thread = new HANDLE[count];
		for (auto index = 0; index < count; ++index)
			thread[index] = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, read_copy_update_func, reinterpret_cast<void*>(&rcu), 0, 0));

		system("pause");
		stop = true;

		WaitForMultipleObjects(count, thread, true, INFINITE);
		for (auto index = 0; index < count; ++index)
			CloseHandle(thread[index]);
		delete[] thread;

		for (auto index = 0; index < count; ++index)
			delete value[index];
		delete[] value;
	}
}