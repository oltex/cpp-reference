#pragma once
#include <Windows.h>
#include <process.h>

#include <tuple>
#include <type_traits>
#include <intrin.h>

#include <iostream>

namespace thread {
	class thread final {
	private:
		template <typename tuple, size_t... index>
		inline static unsigned int __stdcall invoke(void* arg) noexcept {
			const std::unique_ptr<tuple> value(static_cast<tuple*>(arg));
			tuple& tuple = *value.get();
			std::invoke(std::move(std::get<index>(tuple))...);
			return 0;
		}
		template <typename tuple, size_t... index> // 1 2 3 4 5
		static constexpr auto make(std::index_sequence<index...>) noexcept {
			return &invoke<tuple, index...>;
		}
	public:
		inline explicit thread(void) noexcept
			: _thread(GetCurrentThread()) {
		}
		template <typename function, typename... argument>
		inline explicit thread(function&& func, argument&&... arg) noexcept {
			using tuple = std::tuple<std::decay_t<function>, std::decay_t<argument>...>;
			auto copy = std::make_unique<tuple>(std::forward<function>(func), std::forward<argument>(arg)...);
			constexpr auto proc = make<tuple>(std::make_index_sequence<1 + sizeof...(argument)>());
			_thread = (HANDLE)_beginthreadex(nullptr, 0, proc, copy.get(), 0, 0);

			if (_thread)
				copy.release();
			else
				__debugbreak();
		}
		inline ~thread(void) noexcept {
			CloseHandle(_thread);
		}
	public:
		inline void join(void) noexcept {

		}
		inline void detach(void) noexcept {
			CloseHandle(_thread);
		}
		inline unsigned long id(void) noexcept {
			GetThreadId(_thread);
		}

	public:
		inline void set_affinity_mask(DWORD_PTR mask) noexcept {
			SetThreadAffinityMask(_thread, mask);
		}
		inline void set_priority(int const priority) noexcept {
			SetThreadPriority(_thread, priority);
		}
		inline void suspend(void) noexcept {
			SuspendThread(_thread);
		}
		inline void resume(void) noexcept {
			ResumeThread(_thread);
		}
		inline void terminate(void) noexcept {
			TerminateThread(_thread, 0);
		}
		inline void switch_to(void) noexcept {
			SwitchToThread();
		}
	public:
		inline auto data(void) const noexcept -> HANDLE {
			return _thread;
		}
	private:
		HANDLE _thread;
	};
}