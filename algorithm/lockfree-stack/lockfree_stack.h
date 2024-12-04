#pragma once
#include "../../module/multi/spin.h"
#include "../../module/multi/wait_on_address_lock.h"
#include "logger.h"
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
		void* _current = 0;
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
		node* current = reinterpret_cast<node*>(malloc(sizeof(node)));
		current->_value = value;
		for (;;) {
			current->_next = _head;

			node* head = (node*)_InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), current, current->_next);
			if (current->_next == head) {
				//{
				//	auto order = _InterlockedIncrement(&_order);
				//	_log[order]._thread_id = GetCurrentThreadId();
				//	_log[order]._action = L"push : if (current->_next == head) turn";
				//	_log[order]._head = (void*)head;
				//	_log[order]._current = (void*)current;
				//}
				break;
			}
		}
	}
	inline auto pop(void) noexcept -> int {
		int result;
		for (;;) {
			auto current = _head;
			auto next = current->_next;

			node* head = (node*)_InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), next, current);
			if (current == head) {
				{
					auto order = _InterlockedIncrement(&_order);
					_log[order]._thread_id = GetCurrentThreadId();
					_log[order]._action = L"pop : if (current == head) true";
					_log[order]._head = (void*)head;
					_log[order]._next = (void*)next;
					_log[order]._current = (void*)current;
					//if (head->_next != next)
					//	__debugbreak();
				}
				result = current->_value;
				free(current);
				return result;
			}
		}
	}
private:
	node* _head;
	volatile unsigned int _order = 0;
	log* _log = new log[30000000];
};
