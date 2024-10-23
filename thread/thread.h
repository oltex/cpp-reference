#pragma once
#include <Windows.h>

namespace thread {
	//template <class _Tuple, size_t... _Indices>
	//static unsigned int __stdcall _Invoke(void* _RawVals) noexcept /* terminates */ {
	//	// adapt invoke of user's callable object to _beginthreadex's thread procedure
	//	const unique_ptr<_Tuple> _FnVals(static_cast<_Tuple*>(_RawVals));
	//	_Tuple& _Tup = *_FnVals.get(); // avoid ADL, handle incomplete types
	//	_STD invoke(_STD move(_STD get<_Indices>(_Tup))...);
	//	_Cnd_do_broadcast_at_thread_exit(); // TRANSITION, ABI
	//	return 0;
	//}

	//template <class _Tuple, size_t... _Indices>
	//_NODISCARD static constexpr auto _Get_invoke(index_sequence<_Indices...>) noexcept {
	//	return &_Invoke<_Tuple, _Indices...>;
	//}

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