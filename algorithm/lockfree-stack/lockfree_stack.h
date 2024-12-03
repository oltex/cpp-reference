#pragma once
#include "logger.h"
#include <utility>
#include <Windows.h>
#include <intrin.h>

class lockfree_stack final {
private:
	using size_type = unsigned int;
	struct node final {
		node* _next;
		int _value;
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
			_logger.log(L"[thread : %05d] push doing : %08d | start   | currrent->next = _head", GetCurrentThreadId(), value);
			current->_next = _head;
			_logger.log(L"[thread : %05d] push doing : %08d | end     | currrent->next = _head", GetCurrentThreadId(), value);
			_logger.log(L"[thread : %05d] push doing : %08d | start   | _head == current->next ? current", GetCurrentThreadId(), value);
			auto head = _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), current, current->_next);
			_logger.log(L"[thread : %05d] push doing : %08d | end     | _head == current->next ? current", GetCurrentThreadId(), value);
			if (current->_next == head) {
				_logger.log(L"[thread : %05d] push doing : %08d | success", GetCurrentThreadId(), value);
				break;
			}
			else
				_logger.log(L"[thread : %d] push doing : %08d | fail", GetCurrentThreadId(), value);
		}
	}
	inline auto pop(void) noexcept -> int {
		int value;
		for (;;) {
			_logger.log(L"[thread : %05d] pop  doing : 00000000 | start   | current = _head", GetCurrentThreadId());
			auto current = _head;
			value = current->_value;
			_logger.log(L"[thread : %05d] pop  doing : %08d | start   | current = _head", GetCurrentThreadId(), value);
			_logger.log(L"[thread : %05d] pop  doing : %08d | start   | _head == current ? current->next", GetCurrentThreadId(), value);
			auto head = _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), current->_next, current);
			_logger.log(L"[thread : %05d] pop  doing : %08d | end     | _head == current ? current->next", GetCurrentThreadId(), value);
			if (current == head) {
				_logger.log(L"[thread : %05d] pop  doing : %08d | success", GetCurrentThreadId(), value);
				delete current;
				return value;
			}
			else {
				_logger.log(L"[thread : %05d] pop  doing : %08d | fail", GetCurrentThreadId(), value);
			}
		}
	}
private:
	node* _head;
	logger& _logger = logger::instance();
};
