#pragma once
#include "../../data-structure/lockfree/memory-pool/memory_pool.h"

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
		unsigned long long _next;
		int _value;
	};
	struct log final {
		unsigned long _thread_id;
		wchar_t const* _action;
		void* _head = 0;
		void* _tail = 0;
		void* _current = 0;
		void* _next = 0;
	};
public:
	inline explicit lockfree_queue(void) noexcept {
		node* current = &_memory_pool.allocate();
		current->_next = _nullptr = _InterlockedIncrement(&_static_nullptr);
		_head = _tail = reinterpret_cast<unsigned long long>(current);
	}
	inline explicit lockfree_queue(lockfree_queue const& rhs) noexcept = delete;
	inline explicit lockfree_queue(lockfree_queue&& rhs) noexcept = delete;
	inline auto operator=(lockfree_queue const& rhs) noexcept -> lockfree_queue & = delete;
	inline auto operator=(lockfree_queue&& rhs) noexcept -> lockfree_queue & = delete;
	inline ~lockfree_queue(void) noexcept = default;
public:
	inline void push(int value) noexcept {
		node* current = &_memory_pool.allocate();
		current->_value = value;

		for (;;) {
			unsigned long long tail = _tail;
			unsigned long long count = 0xFFFF800000000000ULL & tail;
			node* address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & tail);
			unsigned long long next = address->_next;

			if (0x10000 < (0x00007FFFFFFFFFFFULL & next))
				_InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_tail), next, tail);
			else if (_nullptr == (0x00007FFFFFFFFFFFULL & next) && count == (0xFFFF800000000000ULL & next)) {
				unsigned long long next_count = count + 0x0000800000000000ULL;
				unsigned long long next_tail = reinterpret_cast<unsigned long long>(current) + next_count;
				current->_next = next_count + _nullptr;
				if (next == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&address->_next), next_tail, next)) {
					//{
					//	auto order = _InterlockedIncrement(&_order) % 30000000;
					//	_log[order]._thread_id = GetCurrentThreadId();
					//	_log[order]._action = L"push : tail->next = current // tail = current";
					//	_log[order]._tail = (void*)(tail & 0x00007FFFFFFFFFFFULL);
					//	_log[order]._current = (void*)((unsigned long long)current & 0x00007FFFFFFFFFFFULL);
					//}
					break;
				}
			}
		}
	}
	inline auto pop(void) noexcept -> int {
		for (;;) {
			unsigned long long head = _head;
			node* address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
			unsigned long long next = address->_next;

			if (_nullptr == (0x00007FFFFFFFFFFFULL & next)) {
				if (head == _head)
					__debugbreak();
			}
			else if (0x10000 <= (0x00007FFFFFFFFFFFULL & next)) {
				unsigned long long tail = _tail;
				if (tail == head) {
					node* tail_address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & tail);
					unsigned long long tail_next = tail_address->_next;
					if (0x10000 < (0x00007FFFFFFFFFFFULL & tail_next))
						_InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_tail), next, tail);
				}
				else {
					int result = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & next)->_value;
					if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head)) {
						//{
						//	auto order = _InterlockedIncrement(&_order) % 30000000;
						//	_log[order]._thread_id = GetCurrentThreadId();
						//	_log[order]._action = L"pop : _head = next";
						//	_log[order]._head = (void*)(head & 0x00007FFFFFFFFFFFULL);
						//	_log[order]._next = (void*)(next & 0x00007FFFFFFFFFFFULL);
						//}
						_memory_pool.deallocate(*address);
						return result;
					}
				}
			}
		}
	}
private:
	unsigned long long _head;
	unsigned long long _tail;
	unsigned long long _nullptr;
	inline static unsigned long long _static_nullptr = 0;
	data_structure::lockfree::memory_pool<node> _memory_pool;

	volatile unsigned int _order = 0;
	log* _log = new log[30000000];
};