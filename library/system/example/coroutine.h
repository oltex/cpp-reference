#pragma once
#include "../inputoutput_complet_port.h"
#include "../wait_on_address.h"
#include "../coroutine.h"
#include "../slim_read_write_lock.h"
#include "../thread.h"

namespace example {
	class my_awaiter : public library::awaiter {
	public:
		inline explicit my_awaiter(void) noexcept = default;
		inline explicit my_awaiter(my_awaiter const&) noexcept = delete;
		inline explicit my_awaiter(my_awaiter&&) noexcept = delete;
		inline auto operator=(my_awaiter const&) noexcept -> my_awaiter & = delete;
		inline auto operator=(my_awaiter&&) noexcept -> my_awaiter & = delete;
		inline ~my_awaiter(void) noexcept = default;

		inline bool await_ready(void) noexcept {
			return false;
		}
		inline void await_suspend(std::coroutine_handle<void> handle) noexcept {
			int a = 10;
		}
	};

	class my_promise final : public library::promise<my_promise> {
	public:
		inline explicit my_promise(void) noexcept = default;
		inline explicit my_promise(my_promise const&) noexcept = delete;
		inline explicit my_promise(my_promise&&) noexcept = delete;
		inline auto operator=(my_promise const&) noexcept -> my_promise & = delete;
		inline auto operator=(my_promise&&) noexcept -> my_promise & = delete;
		inline ~my_promise(void) noexcept = default;

		inline auto initial_suspend(void) noexcept -> std::suspend_always {
			return std::suspend_always();
		}
	};
	using my_coroutine = library::coroutine<my_promise>;

	inline my_coroutine function(void) noexcept {
		co_return;
	}
#pragma optimize("", off)
	__declspec(noinline) void function2(void) noexcept {
		return;
	}
#pragma optimize("", on)
	inline my_coroutine function3(void) noexcept {
		for (auto index = 0; index < 100000; ++index)
			co_await my_awaiter();
		co_return;
	}

	inline my_coroutine function4(void) noexcept {
		co_await my_awaiter();
		co_return;
	}
	auto co = function4();


	inline void thread1(void) noexcept {
		co.resume();
	}
	inline void thread2(void) noexcept {
		co.resume();
	}

	inline void coroutine(void) noexcept {
		//library::inputoutput_complet_port iocp;
		//iocp.create(16);
		//{
		//	auto rdtsc = __rdtsc();
		//	for (auto index = 0; index < 100000; ++index)
		//		iocp.post_queue_state(0, 0, 0);
		//	std::printf("pqcs                : %lld\n", __rdtsc() - rdtsc);
		//}
		//{
		//	int a;
		//	auto rdtsc = __rdtsc();
		//	for (auto index = 0; index < 100000; ++index)
		//		library::wake_by_address_single(a);
		//	std::printf("woad                : %lld\n", __rdtsc() - rdtsc);
		//}
		//{
		//	auto rdtsc = __rdtsc();
		//	for (auto index = 0; index < 100000; ++index)
		//		auto coroutine = function();
		//	std::printf("coroutine call      : %lld\n", __rdtsc() - rdtsc);
		//}
		//{
		//	auto rdtsc = __rdtsc();
		//	for (auto index = 0; index < 100000; ++index)
		//		function2();
		//	std::printf("function call       : %lld\n", __rdtsc() - rdtsc);
		//}
		//{
		//	auto rdtsc = __rdtsc();
		//	auto coroutine = function3();
		//	for (auto index = 0; index < 100000; ++index)
		//		coroutine.resume();
		//	std::printf("coroutine lock      : %lld\n", __rdtsc() - rdtsc);
		//}
		//{
		//	library::slim_read_write_lock lock;
		//	auto rdtsc = __rdtsc();
		//	//library::thread::switch_to();
		//	for (auto index = 0; index < 100000; ++index) {
		//		//lock.acquire_exclusive();
		//		//lock.release_exclusive();
		//		Sleep(0);
		//	}
		//	std::printf("context_switch      : %lld\n", __rdtsc() - rdtsc);
		//}

		library::thread t1(thread1, 0);
		library::thread t2(thread2, 0);
		library::sleep(INFINITE);
	}

}