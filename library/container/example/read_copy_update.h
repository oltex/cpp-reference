#pragma once
#include "../read_copy_update.h"
#include "../pool.h"
#include "../list.h"
#include "../lockfree/queue.h"
#include "../../debug.h"
#include <thread>
#include <variant>
#include <utility>

struct mystr : public library::rcu_base<> {
	mystr(void) noexcept
		: value(new int(10)) {
	}
	~mystr(void) noexcept {
		delete value;
	}
	alignas(64) int* value;
};
template<>
inline constexpr bool library::relocate_safe<library::rcu_pointer<mystr>> = true;

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
					value2[index].invalid = true; //제네레이션을 증가시킴 => 이제부터 이포인터를 전 제네레이션으로 참조하고있던애들은 무효다 : 즉 인벨리드가아니다 
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
				auto pointer = value2[other_index]; //만약 상대가 바꾸고 나서 봤다면 내가 에폭시는 빠르지만 제네레이션이 다를수있음 -> 그건 문제가아님 
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



	library::pool<mystr, true, false> _pool;
	library::lockfree::queue<library::rcu_pointer<mystr>> _queue;
	inline static unsigned int __stdcall read_copy_update_make(void* arg) noexcept {
		library::read_copy_update& rcu = library::read_copy_update::instance();

		while (true) {
			rcu.lock();
			if (rand() % 2) {
				auto pointer = _pool.allocate();
				library::rcu_pointer<mystr> rcu_pointer(pointer);
				_queue.emplace(rcu_pointer);

				Sleep(1);
				//_pool.deallocate(pointer);
				rcu_pointer.invalid([&](mystr* pointer) { _pool.deallocate(pointer); });
			}
			rcu.unlock();
		}
		return 0;
	}
	inline static unsigned int __stdcall read_copy_update_read(void* arg) noexcept {
		library::read_copy_update& rcu = library::read_copy_update::instance();
		library::list<library::rcu_pointer<mystr>> list;
		while (true) {
			rcu.lock();
			while (true)
				if (auto result = _queue.pop(); result)
					list.emplace_back(*result);
				else
					break;

			for (auto iter = list.begin(); iter != list.end();) {
				if (iter->valid()) {
					printf("a\n");
					auto result = *(*iter)->value;
					if (result != 10)
						__debugbreak();
				}
				else {
					printf("b\n");
					iter = list.erase(iter);
				}
			}

			rcu.unlock();
		}

		return 0;
	}


	struct mystr3 : public library::rcu_base<> {
		mystr3(void) noexcept
			: value(10) {
		}
		int value;
	};
	struct mystr4 : public mystr3 {
		mystr4(void) noexcept
			: value(20) {
		}
		int value;
	};
	inline void read_copy_update(void) noexcept {
		library::rcu_pointer<mystr3> c;
		library::rcu_pointer<mystr4> c2(c);
		std::variant<int, library::rcu_pointer<mystr3>> v;
		std::variant<int, library::rcu_pointer<mystr4>> v2{ std::in_place_type<library::rcu_pointer<mystr4>> };
		v.emplace<library::rcu_pointer<mystr3>>( std::get<library::rcu_pointer<mystr4>>(v2));
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

			//================

			//scanf_s("%d", &count);
			//HANDLE* thread = new HANDLE[count];
			//for (auto index = 0; index < count; ++index)
			reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, read_copy_update_make, nullptr, 0, 0));
		reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, read_copy_update_read, nullptr, 0, 0));
		system("pause");

	}
}