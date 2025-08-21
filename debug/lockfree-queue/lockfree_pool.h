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
