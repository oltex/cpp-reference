#pragma once
#include <process.h>
#include <Windows.h>
#include <tuple>
#include <type_traits>
#include <memory>

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
		template <typename tuple, size_t... index>
		inline static constexpr auto make(std::index_sequence<index...>) noexcept {
			return &invoke<tuple, index...>;
		}
	public:
		inline explicit thread(void) noexcept
			: _thread(GetCurrentThread()) {
		}
		template <typename function, typename... argument>
		inline explicit thread(function&& func, unsigned int flag, argument&&... arg) noexcept {
			using tuple = std::tuple<std::decay_t<function>, std::decay_t<argument>...>;
			auto copy = std::make_unique<tuple>(std::forward<function>(func), std::forward<argument>(arg)...);
			constexpr auto proc = make<tuple>(std::make_index_sequence<1 + sizeof...(argument)>());
			_thread = (HANDLE)_beginthreadex(nullptr, 0, proc, copy.get(), flag, 0);

			if (_thread)
				copy.release();
			else
				__debugbreak();
		}
		inline explicit thread(thread const& rhs) noexcept = delete;
		inline explicit thread(thread&& rhs) noexcept = delete;
		inline auto operator=(thread const& rhs) noexcept -> thread & = delete;
		inline auto operator=(thread&& rhs) noexcept -> thread & = delete;
		inline ~thread(void) noexcept {
			CloseHandle(_thread);
		}
	public:
		inline void join(unsigned long milli_second) noexcept {
			WaitForSingleObject(_thread, milli_second);
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
		inline void wait_for_single_object(unsigned long milli_second) noexcept {
			WaitForSingleObject(_thread, milli_second);
		}
		inline auto get_exit_code(void) noexcept -> unsigned long {
			unsigned long code;
			GetExitCodeThread(_thread, &code);
			return code;
		}
	public:
		inline static void switch_to(void) noexcept {
			SwitchToThread();
		}
		inline static auto current_id(void) noexcept {
			return GetCurrentThreadId();
		}
		//inline static void wait_for_multiple_object(std::initializer_list<thread> threads, bool wait_all, unsigned long milli_second) noexcept {
		//	HANDLE handle;
		//	for (auto& iter : threads) {
		//		iter.data();
		//	}
		//	WaitForMultipleObjects(threads.size(), );
		//}
	public:
		inline auto data(void) const noexcept -> HANDLE {
			return _thread;
		}
	private:
		HANDLE _thread;
	};
}