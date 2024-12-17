#pragma once
#include "../../data-structure/lockfree-object-pool/lockfree_object_pool.h"
#include "../../data-structure/lockfree-memory-pool/lockfree_memory_pool.h"

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

		void* _tail = 0;
		void* _current = 0;
		void* _head = 0;
		void* _next = 0;
	};
public:
	inline explicit lockfree_queue(void) noexcept {
		node* current = &_object_pool.allocate();
		current->_next = nullptr;
		_head = _tail = reinterpret_cast<unsigned long long>(current);
	}
	inline explicit lockfree_queue(lockfree_queue const& rhs) noexcept = delete;
	inline explicit lockfree_queue(lockfree_queue&& rhs) noexcept = delete;
	inline auto operator=(lockfree_queue const& rhs) noexcept -> lockfree_queue & = delete;
	inline auto operator=(lockfree_queue&& rhs) noexcept -> lockfree_queue & = delete;
	inline ~lockfree_queue(void) noexcept = default;
public:
	inline void push(int value) noexcept {
		node* current = &_object_pool.allocate();
		current->_value = value;
		current->_next = nullptr;

		for (;;) {
			unsigned long long tail = _tail;
			node* address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & tail);
			node* next = address->_next;

			if (nullptr != next)
				_InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_tail),
					reinterpret_cast<unsigned long long>(next) + (0xFFFF800000000000ULL & tail) + 0x0000800000000000ULL, tail);
			else if (nullptr == _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&address->_next), (void*)current, nullptr)) {
				{
					auto order = _InterlockedIncrement(&_order) % 30000000;
					_log[order]._thread_id = GetCurrentThreadId();
					_log[order]._action = L"push : tail->next = current // tail = current";
					_log[order]._tail = (void*)(tail);
					_log[order]._current = (void*)((unsigned long long)current & 0x00007FFFFFFFFFFFULL);
					_log[order]._head = 0;
					_log[order]._next = 0;
				}
				break;
			}
		}
	}
	inline auto pop(void) noexcept -> int {
		for (;;) {
			unsigned long long head = _head;
			node* address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
			node* next = address->_next;
			if (next == nullptr)
				continue;

			unsigned long long tail = _tail;
			node* tail_address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & tail);
			if (reinterpret_cast<unsigned long long>(tail_address) == reinterpret_cast<unsigned long long>(address)) {
				node* next2 = tail_address->_next;
				if (nullptr != next2)
					_InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_tail),
						reinterpret_cast<unsigned long long>(next2) + (0xFFFF800000000000ULL & tail) + 0x0000800000000000ULL, tail);
			}
			else {
				int result = next->_value;
				unsigned long long change = reinterpret_cast<unsigned long long>(next) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), change, head)) {
					{
						auto order = _InterlockedIncrement(&_order) % 30000000;
						_log[order]._thread_id = GetCurrentThreadId();
						_log[order]._action = L"pop : _head = next";
						_log[order]._tail = 0;
						_log[order]._current = 0;
						_log[order]._head = (void*)(head & 0x00007FFFFFFFFFFFULL);
						_log[order]._next = (void*)(change & 0x00007FFFFFFFFFFFULL);
					}
					_object_pool.deallocate(*address);
					return result;
				}
			}
		}
	}
private:
	unsigned long long _head;
	unsigned long long _tail;
	data_structure::lockfree::memory_pool<node> _object_pool;


	volatile unsigned int _order = 0;
	log* _log = new log[30000000];
};