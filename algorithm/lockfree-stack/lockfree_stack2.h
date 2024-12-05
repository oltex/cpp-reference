#pragma once
#include "../../module/multi/spin.h"
#include "../../module/multi/wait_on_address_lock.h"
#include <utility>
#include <Windows.h>
#include <intrin.h>
#include <string>
#include <memory>

class lockfree_stack2 final {
private:
	using size_type = unsigned int;
	struct log final {
		unsigned long _thread_id;
		wchar_t const* _action;

		void* _head = 0;
		void* _current = 0;
		void* _next = 0;
	};
public:
	inline explicit lockfree_stack2(void) noexcept {
		_head = reinterpret_cast<LONG64*>(_aligned_malloc(sizeof(LONG64) * 2, 128));
		memset((void*)_head, 0, sizeof(long long) * 2);
		//_head = malloc
	}
	inline explicit lockfree_stack2(lockfree_stack2 const& rhs) noexcept = delete;
	inline explicit lockfree_stack2(lockfree_stack2&& rhs) noexcept = delete;
	inline auto operator=(lockfree_stack2 const& rhs) noexcept -> lockfree_stack2 & = delete;
	inline auto operator=(lockfree_stack2&& rhs) noexcept -> lockfree_stack2 & = delete;
	inline ~lockfree_stack2(void) noexcept = default;
public:
	inline void push(int value) noexcept {
		LONG64* current = reinterpret_cast<LONG64*>(_aligned_malloc(sizeof(LONG64) * 2, 16));
		//current[0] = reinterpret_cast<long long>(malloc(sizeof(int)));
		//*((int*)current[0]) = value;

		for (;;) {
			LONG64 compare[2] = { _head[0], _head[1] };
			if (_InterlockedCompareExchange128(_head, compare[0], (LONG64)current, compare)) {
				break;
			}
			//if (current->_next == head) {
			//	//{
			//	//	auto order = _InterlockedIncrement(&_order);
			//	//	_log[order]._thread_id = GetCurrentThreadId();
			//	//	_log[order]._action = L"push : if (current->_next == head) turn";
			//	//	_log[order]._head = (void*)head;
			//	//	_log[order]._current = (void*)current;
			//	//}
			//	break;
			//}
		}
	}
	inline auto pop(void) noexcept -> int {
		for (;;) {
			LONG64 compare[2] = { _head[0], _head[1] };
			LONG* current = reinterpret_cast<LONG*>(compare[0]);

			if (_InterlockedCompareExchange128(_head, compare[0], compare[1], compare)) {
				free(current);
			}
		}
		return 0;
	}
private:
	__declspec(align(16)) volatile LONG64* _head;
	volatile unsigned int _order = 0;
	log* _log = new log[30000000];
};
