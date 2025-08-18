#pragma once
#include "memory.h"
#include "system/interlock.h"

class push_lock final {
	struct node {
		node* _next;
		node* _last;
		node* _prev;
		unsigned long long _flag;
		unsigned long long _share;
	};
	unsigned long long _head;
	inline static constexpr unsigned long long multi = 0x8000000000000000ULL;
	inline static constexpr unsigned long long wake = 0x4000000000000000ULL;// k
	inline static constexpr unsigned long long stack = 0x2000000000000000ULL;
	inline static constexpr unsigned long long exclusive = 0x1000000000000000ULL;
	inline static constexpr unsigned long long share = 0x00007FFFFFFFFFFFULL;
public:
	inline explicit push_lock(void) noexcept
		: _head(0) {
	};
	inline explicit push_lock(push_lock const&) noexcept = delete;
	inline explicit push_lock(push_lock&&) noexcept = delete;
	inline auto operator=(push_lock const&) noexcept -> push_lock & = delete;
	inline auto operator=(push_lock&&) noexcept -> push_lock & = delete;
	inline ~push_lock(void) noexcept = default;

	inline void acquire_exclusive(void) noexcept {
		for (;;) {
			auto head = _head;
			if (false == (head & exclusive)) {
				if (head == library::interlock_compare_exhange(_head, head | exclusive, head))
					break;
			}
			else {
				auto current = library::allocate<node>();
				printf("%p\n", current);
				//library::construct<type>();
				unsigned long long next;

				if (false == (head & stack)) {
					current->_last = current;
					current->_share = _head & share;
					if (1 >= current->_share) {
						current->_share = 0;
						next = stack | exclusive | reinterpret_cast<unsigned long long>(current);
					}
					else
						next = multi | stack | exclusive | reinterpret_cast<unsigned long long>(current);
				}
				else {
					current->_last = nullptr;
					current->_next = reinterpret_cast<node*>(head & share);
					current->_share = 0;
					next = exclusive | reinterpret_cast<unsigned long long>(current) | (head & (multi | wake | stack | exclusive));
				}

				if (head == library::interlock_compare_exhange(_head, next, head))
					break;
			}
		}
	}
	inline void acquire_share(void) noexcept {
		for (;;) {
			auto head = _head;
			if (false == (head & exclusive) || (false == (head & stack) && 0 < (head & share))) {
				unsigned long long next;
				if (true == (head & stack)) 
					next = head + exclusive;
				else 
					next = (head + 1) | exclusive;
				if (head == library::interlock_compare_exhange(_head, next, head))
					break;
			}
			else {

			}
		}
	}
	inline void release_exclusive(void) noexcept {
		for (;;) {
			auto lock = _head;
			unsigned long long next;
			if (false == (lock & stack) || true == (lock & wake)) {
				next = lock - exclusive;
				if (lock == library::interlock_compare_exhange(_head, next, lock))
					break;
			}
			else {
				next = lock - exclusive + wake;
				if (lock == library::interlock_compare_exhange(_head, next, lock)) {
					wake_thread(next);
					break;
				}
			}
		}
	}

private:
	inline void wake_thread(unsigned long long lock) noexcept {
		node* first;
		node* prev;
		node* last;
		auto current = reinterpret_cast<node*>(lock & share);

		first = current;
		while (1) {
			last = current->_last;
			if (last != NULL) {
				current = last;
				break;
			}
			prev = current;
			current = current->_next;
			current->_prev = prev;
		}


	}
};