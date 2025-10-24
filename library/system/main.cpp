#include "../debug.h"

#include "file.h"
//#include "example/interlock.h"
//#include "example/coroutine.h"
#include "type_info.h"
#include "thread.h"
#include "interlock.h"

int value = 0;

inline auto func() noexcept {
	for (auto index = 0; index < 100000; ++index) {
		//library::interlock_increment(value);
		++value;
	}
}

int main(void) noexcept {
	library::crt_set_debug_flag();
	library::thread t[10];

	system("pause");
	auto rdtec = __rdtsc();
	for (auto index = 0; index < 10; ++index) {
		t[index] = std::move(library::thread(func, 0));
	}

	for (auto index = 0; index < 10; ++index) {
		t[index].wait_for_single(INFINITE);
	}

	printf("%lld", __rdtsc() - rdtec);
	

	//example::interlock();
	//example::coroutine();

	return 0;
}