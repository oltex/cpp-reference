#pragma once
#include "lockfree_pool.h"

class lockfree_queue final {
private:
	using size_type = unsigned int;
	struct node final {
		inline explicit node(void) noexcept = delete;
		inline explicit node(node const&) noexcept = delete;
		inline explicit node(node&&) noexcept = delete;
		inline auto operator=(node const&) noexcept = delete;
		inline auto operator=(node&&) noexcept = delete;
		inline ~node(void) noexcept = delete;
		node* _next;
		int _value;
	};
	struct log final {
		unsigned long _thread_id;
		wchar_t const* _action;
		void* _head;
		void* _tail;
		void* _next;
	};
public:
	inline explicit lockfree_queue(void) noexcept {
		node* current = &_pool.allocate();
		current->_next = nullptr;
		_head = _tail = reinterpret_cast<unsigned long long>(current);
	}
	inline explicit lockfree_queue(lockfree_queue const&) noexcept = delete;
	inline explicit lockfree_queue(lockfree_queue&&) noexcept = delete;
	inline auto operator=(lockfree_queue const&) noexcept -> lockfree_queue & = delete;
	inline auto operator=(lockfree_queue&&) noexcept -> lockfree_queue & = delete;
	inline ~lockfree_queue(void) noexcept = default;

	inline void push(int value) noexcept {
		node* current = &_pool.allocate();
		current->_value = value;
		current->_next = nullptr;

		for (;;) {
			unsigned long long tail = _tail;
			node*& next = ((node*)(0x00007FFFFFFFFFFFULL & tail))->_next;

			if (nullptr == _InterlockedCompareExchangePointer(
				(void**)&next, (void*)current, nullptr)) {
				auto order = _InterlockedIncrement(&_order) % 10000000;
				_log[order]._thread_id = GetCurrentThreadId();
				_log[order]._action = L"push 2";
				_log[order]._head = nullptr;
				_log[order]._tail = (void*)tail;
				_log[order]._next = (void*)current;
				unsigned long long next_tail = (unsigned long long)current +
					(0xFFFF800000000000ULL & tail) + 0x0000800000000000ULL;
				if (tail == _InterlockedCompareExchange(&_tail, next_tail, tail)) {
					auto order = _InterlockedIncrement(&_order) % 10000000;
					_log[order]._thread_id = GetCurrentThreadId();
					_log[order]._action = L"push";
					_log[order]._head = nullptr;
					_log[order]._tail = (void*)tail;
					_log[order]._next = (void*)next_tail;
				}
				break;
			}
		}
	}
	inline auto pop(void) noexcept -> int {
		for (;;) {
			unsigned long long head = _head;
			node* address = (node*)(0x00007FFFFFFFFFFFULL & head);
			node* next = address->_next;

			if (nullptr == next)
				__debugbreak();

			unsigned long long next_head = (unsigned long long)next
				+ (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
			if (head == _InterlockedCompareExchange(&_head, next_head, head)) {
				#pragma region log
				auto order = _InterlockedIncrement(&_order) % 10000000;
				_log[order]._thread_id = GetCurrentThreadId();
				_log[order]._action = L"pop";
				_log[order]._head = (void*)head;
				_log[order]._tail = nullptr;
				_log[order]._next = (void*)next_head;
				#pragma endregion
				int result = next->_value;
				_pool.deallocate(*address);
				return result;
			}
		}
	}
private:
	unsigned long long _head;
	unsigned long long _tail;
	lockfree_pool<node> _pool;
	log* _log = new log[10000000];
	unsigned int _order = 0;
};