#include "push_lock.h"
#include "library/system/thread.h"

library::push_lock push_lock;
int value = 0;
int share = 0;

inline void function1(void) noexcept {
	for (;;) {
		push_lock.acquire_exclusive();
		if (0 != value)
			__debugbreak();
		++value;
		if (1 != value)
			__debugbreak();
		--value;
		//printf("function1\n");
		push_lock.release_exclusive();
		Sleep(0);
	}
}
inline void function2(void) noexcept {
	for (;;) {
		push_lock.acquire_share();
		if (0 != value)
			__debugbreak();
		//printf("function2\n");
		auto res = library::interlock_increment(share);
		printf("%d\n", res);
		library::interlock_decrement(share);
		push_lock.release_share();
		Sleep(0);
	}
}
inline void function3(void) noexcept {
	for (;;) {
		push_lock.acquire_share();
		if (0 != value)
			__debugbreak();
		//printf("function3\n");
		auto res = library::interlock_increment(share);
		printf("%d\n", res);
		library::interlock_decrement(share);
		push_lock.release_share();
		Sleep(0);
	}
}

int main(void) noexcept {
	library::thread thread1(function1, 0);
	library::thread thread2(function2, 0);
	library::thread thread3(function3, 0);
	library::sleep(INFINITE);
	return 0;
}