#pragma once
#include "../../data-structure/lockfree-memory-pool/lockfree_memory_pool.h"

class lockfree_queue final {
private:
	using size_type = unsigned int;
	struct node final {
		node* _next;
		int _value;
	};
public:
	inline explicit lockfree_queue(void) noexcept
		: _head(0), _tail(0) {
	}
	inline explicit lockfree_queue(lockfree_queue const& rhs) noexcept = delete;
	inline explicit lockfree_queue(lockfree_queue&& rhs) noexcept = delete;
	inline auto operator=(lockfree_queue const& rhs) noexcept -> lockfree_queue & = delete;
	inline auto operator=(lockfree_queue&& rhs) noexcept -> lockfree_queue & = delete;
	inline ~lockfree_queue(void) noexcept = default;
public:
	inline void push(int value) noexcept {
		//node* current = reinterpret_cast<node*>(malloc(sizeof(node)));

	}
	inline auto pop(void) noexcept -> int {
	}
private:
	unsigned long long _head;
	unsigned long long _tail;
	data_structure::lockfree::memory_pool<node> _memory_pool;
};