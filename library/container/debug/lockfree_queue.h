#pragma once
#include <memory>
#include <Windows.h>

template<typename type>
class lockfree_pool final {
private:
	struct node final {
		inline explicit node(void) noexcept = delete;
		inline explicit node(node const&) noexcept = delete;
		inline explicit node(node&&) noexcept = delete;
		inline auto operator=(node const&) noexcept = delete;
		inline auto operator=(node&&) noexcept = delete;
		inline ~node(void) noexcept = delete;
		node* _next;
		type _value;
	};
public:
	inline explicit lockfree_pool(void) noexcept
		: _head(0) {
	};
	inline explicit lockfree_pool(lockfree_pool const&) noexcept = delete;
	inline explicit lockfree_pool(lockfree_pool&& rhs) noexcept
		: _head(rhs._head) {
		rhs._head = nullptr;
	};
	inline auto operator=(lockfree_pool const&) noexcept = delete;
	inline auto operator=(lockfree_pool&& rhs) noexcept {
		_head = rhs._head;
		rhs._head = nullptr;
	}
	inline ~lockfree_pool(void) noexcept {
		node* head = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & _head);
		while (nullptr != head) {
			node* next = head->_next;
			free(head);
			head = next;
		}
	}
public:
	template<typename... argument>
	inline auto allocate(argument&&... arg) noexcept -> type& {
		node* current;
		for (;;) {
			unsigned long long head = _head;
			current = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
			if (nullptr == current) {
				current = reinterpret_cast<node*>(::malloc(sizeof(node)));
				break;
			}
			unsigned long long next = reinterpret_cast<unsigned long long>(current->_next) + (0xFFFF800000000000ULL & head);
			if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
				break;
		}
		return current->_value;
	}
	inline void deallocate(type& value) noexcept {
		node* current = reinterpret_cast<node*>(reinterpret_cast<unsigned char*>(&value) - offsetof(node, _value));
		for (;;) {
			unsigned long long head = _head;
			current->_next = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
			unsigned long long next = reinterpret_cast<unsigned long long>(current) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
			if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
				break;
		}
	}
private:
	alignas(64) unsigned long long _head;
};

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

#include <thread>
#include <intrin.h>
#include <Windows.h>
#include <intrin.h>
#include <iostream>

lockfree_queue _lockfree_queue;

inline static unsigned int __stdcall func(void* arg) noexcept {
	int count = 0;
	for (;;) {
		if (count++ == 100000) {
			printf("thread : %d\n", GetCurrentThreadId());
			count = 0;
		}
		for (int i = 0; i < 2; ++i)
			_lockfree_queue.push(0);
		for (int i = 0; i < 2; ++i)
			auto value = _lockfree_queue.pop();
	}
}

int main(void) noexcept {
	_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	_beginthreadex(nullptr, 0, func, nullptr, 0, 0);
	Sleep(INFINITE);
	return 0;
}

//inline static unsigned int __stdcall func_tailtail1(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.push(1);
//		_lockfree_queue.pop();
//	}
//}
//inline static unsigned int __stdcall func_tailtail2(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.pop();
//		_lockfree_queue.push(1);
//	}
//}
//
//inline static unsigned int __stdcall func_pop1(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.pop();
//	}
//}
//inline static unsigned int __stdcall func_pop2(void* arg) noexcept {
//	for (;;) {
//		_lockfree_queue.pop();
//		_lockfree_queue.push(1);
//	}
//}
//
//inline static unsigned int __stdcall func_push(void* arg) noexcept {
//	unsigned long long count = 0;
//	for (;;) {
//		_lockfree_queue.push(count);
//		count++;
//	}
//}
//inline static unsigned int __stdcall func_pop(void* arg) noexcept {
//	unsigned long long count = 0;
//	for (;;) {
//		auto a =_lockfree_queue.pop();
//		if (a == 0) {
//
//		}
//		else if (count != a)
//			__debugbreak();
//		count = a + 1;
//	}
//}