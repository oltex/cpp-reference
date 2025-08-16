#pragma once
#include "../inputoutput_complet_port.h"
#include "../wait_on_address.h"
#include "../coroutine.h"
#include "../slim_read_write_lock.h"
#include "../thread.h"

namespace example {

	inline library::coroutine<library::promise> function(void) noexcept {
		co_return;
	}
#pragma optimize("", off)
	__declspec(noinline) void function2(void) noexcept {
		return;
	}
#pragma optimize("", on)
	inline library::coroutine<library::promise> function3(void) noexcept {
		for (auto index = 0; index < 100000; ++index)
			co_await library::suspend();
		co_return;
	}

	inline void coroutine(void) noexcept {
		library::inputoutput_complet_port iocp;
		iocp.create(16);
		{
			auto rdtsc = __rdtsc();
			for (auto index = 0; index < 100000; ++index)
				iocp.post_queue_state(0, 0, 0);
			std::printf("pqcs                : %lld\n", __rdtsc() - rdtsc);
		}

		{
			int a;
			auto rdtsc = __rdtsc();
			for (auto index = 0; index < 100000; ++index)
				library::wake_by_address_single(a);
			std::printf("woad                : %lld\n", __rdtsc() - rdtsc);
		}

		MyStruct* mystr[100000];
		for (auto index = 0; index < 100000; ++index)
			mystr[index] = _pool.allocate();
		for (auto index = 0; index < 100000; ++index)
			_pool.deallocate(mystr[index]);
		{
			auto rdtsc = __rdtsc();
			for (auto index = 0; index < 100000; ++index)
				auto coroutine = function();
			std::printf("coroutine call      : %lld\n", __rdtsc() - rdtsc);
		}
		{
			auto rdtsc = __rdtsc();
			for (auto index = 0; index < 100000; ++index)
				function2();
			std::printf("function call       : %lld\n", __rdtsc() - rdtsc);
		}

		{
			auto rdtsc = __rdtsc();
			auto coroutine = function3();
			for (auto index = 0; index < 100000; ++index)
				coroutine.resume();
			std::printf("coroutine lock      : %lld\n", __rdtsc() - rdtsc);
		}
		{
			library::slim_read_write_lock lock;
			auto rdtsc = __rdtsc();
			//library::thread::switch_to();
			for (auto index = 0; index < 100000; ++index) {
				//lock.acquire_exclusive();
				//lock.release_exclusive();
				Sleep(0);
			}
			std::printf("context_switch      : %lld\n", __rdtsc() - rdtsc);
		}
	}

}