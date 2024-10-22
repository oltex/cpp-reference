#pragma once
#include <Windows.h>

namespace thread {
	class thread final {
	public:
		inline explicit thread(void) noexcept
			: _thread(GetCurrentThread()) {
		}
		template <typename function, typename... argument>
		inline explicit thread(function&& func, argument&&... arg) noexcept {
			_beginthreadex(nullptr, 0, func, arg, 0, 0);
			//reinterpret_cast<void*>(_CSTD _beginthreadex(nullptr, 0, _Invoker_proc, _Decay_copied.get(), 0, &_Thr._Id));
		}
		inline ~thread(void) noexcept = default;
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
		inline DWORD id(void) noexcept {
			GetThreadId(_thread);
		}
	private:
		HANDLE _thread;
	};
}