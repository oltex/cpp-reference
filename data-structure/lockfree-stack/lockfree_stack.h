#pragma once
#include "../lockfree-memory-pool/lockfree_memory_pool.h"
#include <utility>
#include <Windows.h>
#include <intrin.h>

namespace data_structure::lockfree {
	template<typename type>
	class stack final {
	private:
		using size_type = unsigned int;
		struct node final {
			node* _next;
			type _value;
		};
	public:
		inline explicit stack(void) noexcept
			: _head(0) {
		}
		inline explicit stack(stack const& rhs) noexcept = delete;
		inline explicit stack(stack&& rhs) noexcept = delete;
		inline auto operator=(stack const& rhs) noexcept -> stack & = delete;
		inline auto operator=(stack&& rhs) noexcept -> stack & = delete;
		inline ~stack(void) noexcept = default;
	public:
		template<typename universal>
		inline void push(universal&& value) noexcept {
			node* current = &_memory_pool.allocate();
			current->_value = std::forward<universal>(value);

			for (;;) {
				unsigned long long head = _head;
				current->_next = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
				unsigned long long next = reinterpret_cast<unsigned long long>(current) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
					break;
			}
		}
		inline auto pop(void) noexcept -> type {
			for (;;) {
				unsigned long long head = _head;
				node* current = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
				unsigned long long next = reinterpret_cast<unsigned long long>(current->_next) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head)) {
					type result = current->_value;
					_memory_pool.deallocate(*current);
					return result;
				}
			}
		}
	private:
		unsigned long long _head;
		memory_pool<node> _memory_pool;
	};
}