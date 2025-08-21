#include "push_lock.h"
#include "library/system/thread.h"

library::push_lock push_lock;
int value = 0;
int share = 0;

inline void function1(void) noexcept {
	unsigned long long count = 0;
	for (;;) {
		if (count++ == 100) {
			printf("function1 : %d\n", library::thread::get_current_id());
			count = 0;
		}
		push_lock.acquire_exclusive();
		if (0 != value)
			__debugbreak();
		++value;
		if (1 != value)
			__debugbreak();
		--value;
		push_lock.release_exclusive();
		Sleep(rand() % 100);
	}
}
inline void function2(void) noexcept {
	unsigned long long count = 0;
	for (;;) {
		if (count++ == 100) {
			printf("function2 : %d\n", library::thread::get_current_id());
			count = 0;
		}
		push_lock.acquire_share();
		if (0 != value)
			__debugbreak();
		push_lock.release_share();
		Sleep(rand() % 100);
	}
}

int main(void) noexcept {
	library::thread thread1(function1, 0);
	library::thread thread1_2(function1, 0);
	library::thread thread1_3(function1, 0);
	library::thread thread2(function2, 0);
	library::thread thread2_1(function2, 0);
	library::thread thread2_2(function2, 0);
	library::thread thread2_3(function2, 0);
	library::sleep(INFINITE);
	return 0;
}