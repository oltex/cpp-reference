#pragma once
#include <utility>
#include <Windows.h>
#include <intrin.h>
#include <string>

class lockfree_stack final {
private:
	using size_type = unsigned int;
	struct node final {
		node* _next;
		int _value;
	};
	struct log final {
		unsigned long _thread_id;
		wchar_t const* _action;
		void* _head = 0;
		void* _next = 0;
	};
public:
	inline explicit lockfree_stack(void) noexcept
		: _head(nullptr) {
	}
	inline explicit lockfree_stack(lockfree_stack const& rhs) noexcept = delete;
	inline explicit lockfree_stack(lockfree_stack&& rhs) noexcept = delete;
	inline auto operator=(lockfree_stack const& rhs) noexcept -> lockfree_stack & = delete;
	inline auto operator=(lockfree_stack&& rhs) noexcept -> lockfree_stack & = delete;
	inline ~lockfree_stack(void) noexcept = default;
public:
	inline void push(int value) noexcept {
		node* next = reinterpret_cast<node*>(malloc(sizeof(node)));
		next->_value = value;
		for (;;) {
			node* head = _head;
			next->_next = head;
			if (head == (node*)_InterlockedCompareExchangePointer((void* volatile*)(&_head), next, head)) {
				auto order = _InterlockedIncrement(&_order) % 10000000;
				_log[order]._thread_id = GetCurrentThreadId();
				_log[order]._action = L"push";
				_log[order]._head = (void*)head;
				_log[order]._next = (void*)next;
				break;
			}
		}
	}
	inline auto pop(void) noexcept -> int {
		int result;
		for (;;) {
			auto head = _head;
			auto next = head->_next;
			if (head == (node*)_InterlockedCompareExchangePointer((void* volatile*)(&_head), next, head)) {
				#pragma region log
				auto order = _InterlockedIncrement(&_order) % 10000000;
				_log[order]._thread_id = GetCurrentThreadId();
				_log[order]._action = L"pop";
				_log[order]._head = (void*)head;
				_log[order]._next = (void*)next;
#pragma endregion
				result = head->_value;
				free(head);
				return result;
			}
		}
	}
private:
	node* _head;
	volatile unsigned int _order = 0;
	log* _log = new log[10000000]{};
};
