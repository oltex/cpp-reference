#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _WINSOCKAPI_
#include "io_complet_port.h"
#include "thread_pool.h"

#include "library/system/event.h"

int _count;
int function(void) noexcept {
	printf("hello\n");
	library::interlock_increment(_count);
	return 500;
}


int function2(void) noexcept {
	printf("hello 10000\n");
	library::interlock_increment(_count);
	return 10000;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//library::event event(false, false);
	//{
	//	auto rdtsc = __rdtsc();
	//	for (auto index = 0; index < 100; ++index)
	//		event.set();
	//	printf("%lld\n", __rdtsc() - rdtsc);
	//}
	//int pp = 0;
	//{
	//	auto rdtsc = __rdtsc();
	//	for (auto index = 0; index < 100; ++index)
	//		library::wake_by_address_all(pp);
	//	printf("%lld\n", __rdtsc() - rdtsc);
	//}
	//{
	//	auto rdtsc = __rdtsc();
	//	for (auto index = 0; index < 100; ++index)
	//		inst._complet_port.post_queue_state(0, 0, 0);
	//	printf("%lld\n", __rdtsc() - rdtsc);
	//}
	{
		auto& instance = io_complet_port::construct(4, 1);
		//system("pause");
		//for (auto index = 0; index < 64; ++index)
		//instance.execute(function2);
		//instance.execute(function);
		//Sleep(30000);
		//__debugbreak();
		system("pause");
		io_complet_port::destruct();
	}
	//{
	//	auto& instance = thread_pool::construct(16);
	//	system("pause");
	//	for (auto index = 0; index < 64; ++index)
	//		instance.execute(function);
	//	Sleep(30000);
	//	__debugbreak();
	//	system("pause");
	//	thread_pool::destruct();
	//}
}