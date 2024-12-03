#pragma once
#include "../../module/multi/spin.h"
#include "../../module/multi/wait_on_address_lock.h"
#include "logger.h"
#include <utility>
#include <Windows.h>
#include <intrin.h>
#include <string>

class lockfree_stack2 final {
private:
	using size_type = unsigned int;
	struct node final {
		node* _next;
		int _value;
	};
	struct log final {
		std::wstring _action;
		unsigned long _thread;

		uintptr_t _head;
		int _head_value;
		uintptr_t _head_next;
		int _head_next_value;

		uintptr_t _current;
		int _current_value;
		uintptr_t _current_next;
		int _current_next_value;
	};
public:
	inline explicit lockfree_stack2(void) noexcept
		: _head(nullptr) {
	}
	inline explicit lockfree_stack2(lockfree_stack2 const& rhs) noexcept = delete;
	inline explicit lockfree_stack2(lockfree_stack2&& rhs) noexcept = delete;
	inline auto operator=(lockfree_stack2 const& rhs) noexcept -> lockfree_stack2 & = delete;
	inline auto operator=(lockfree_stack2&& rhs) noexcept -> lockfree_stack2 & = delete;
	inline ~lockfree_stack2(void) noexcept = default;
public:
	inline void push(int value) noexcept {
		node* current = reinterpret_cast<node*>(malloc(sizeof(node)));
		current->_value = value;
		for (;;) {
			_spin.lock();
			current->_next = _head;
			_spin.unlock();
			_logger.log(L"[thread : %05d][order : %08d] push : %08d | currrent->next = _head", GetCurrentThreadId(), value);

			_spin.lock();
			auto head = _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), current, current->_next);
			_spin.unlock();
			_logger.log(L"[thread : %05d][order : %08d] push : %08d | _head == current->next ? current", GetCurrentThreadId(), order, value);

			if (current->_next == head) {
				_logger.log(L"[thread : %05d][order : %08d] push : %08d | success", GetCurrentThreadId(), value);
				break;
			}
			else
				_logger.log(L"[thread : %d][order : %08d] push : %08d | fail", GetCurrentThreadId(), value);
		}
	}
	inline auto pop(void) noexcept -> int {
		int value;
		for (;;) {
			_spin.lock();
			auto current = _head;
			value = current->_value;
			_spin.unlock();
			_logger.log(L"[thread : %05d][order : %08d] pop  : %08d | current = _head", GetCurrentThreadId(), value);

			_spin.lock();
			auto next = current->_next;
			_spin.unlock();
			_logger.log(L"[thread : %05d][order : %08d] pop  : %08d | next = current->next", GetCurrentThreadId(), value);

			_spin.lock();
			auto head = _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), next, current);
			_spin.unlock();
			_logger.log(L"[thread : %05d][order : %08d] pop  : %08d | end     | _head == current ? next", GetCurrentThreadId(), value);

			if (current == head) {
				_spin.lock();
				free(current);
				_spin.unlock();
				_logger.log(L"[thread : %05d][order : %08d] pop  : %08d | success", GetCurrentThreadId(), value);
				return value;
			}
			else
				_logger.log(L"[thread : %05d][order : %08d] pop  : %08d | fail", GetCurrentThreadId(), value);
		}
	}
private:
	node* _head;
	logger& _logger = logger::instance();

	multi::lock::spin _spin;
	unsigned int _order = 0;
	log* _log = new log[10000000];
};
