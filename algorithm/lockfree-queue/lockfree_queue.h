#pragma once
#include "../../data-structure/lockfree-object-pool/lockfree_object_pool.h"

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

		void* _head = 0;
		void* _tail = 0;
		void* _current = 0;
		void* _next = 0;
	};
public:
	inline explicit lockfree_queue(void) noexcept {
		node* current = &_object_pool.acquire();
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
		node* current = &_object_pool.acquire();
		current->_value = value;

		for (;;) {
			unsigned long long tail = _tail;
			node*& next = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & tail)->_next;
			current->_next = nullptr;


			if (nullptr == _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&next), current, nullptr)) {
				unsigned long long change = reinterpret_cast<unsigned long long>(current) + (0xFFFF800000000000ULL & tail) + 0x0000800000000000ULL;

				auto old_tail = _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_tail), change, tail);
				{
					auto order = _InterlockedIncrement(&_order);
					_log[order]._thread_id = GetCurrentThreadId();
					_log[order]._action = L"push : tail->next = current // tail = current";
					_log[order]._tail = (void*)(tail & 0x00007FFFFFFFFFFFULL);
					_log[order]._current = (void*)(change & 0x00007FFFFFFFFFFFULL);
				}
				if (tail == old_tail)
					break;
				else
					__debugbreak();
				//auto old_tail = _InterlockedExchange(reinterpret_cast<unsigned long long volatile*>(&_tail), change);
				//if ((old_tail & 0x00007FFFFFFFFFFFULL) != (tail & 0x00007FFFFFFFFFFFULL))
				//	__debugbreak();
				//break;
			}
		}
	}
	inline auto pop(void) noexcept -> int {
		for (;;) {
			unsigned long long head = _head;
			node* current = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
			node* next = current->_next;

			unsigned long long change = reinterpret_cast<unsigned long long>(next) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
			if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), change, head)) {
				{
					auto order = _InterlockedIncrement(&_order);
					_log[order]._thread_id = GetCurrentThreadId();
					_log[order]._action = L"pop : _head = next";
					_log[order]._head = (void*)(head & 0x00007FFFFFFFFFFFULL);
					_log[order]._next = (void*)(change & 0x00007FFFFFFFFFFFULL);
				}
				int result = next->_value;
				_object_pool.release(*current);
				return result;
			}
		}
	}
private:
	unsigned long long _head;
	unsigned long long _tail;
	data_structure::lockfree::object_pool<node> _object_pool;


	volatile unsigned int _order = 0;
	log* _log = new log[30000000];
};