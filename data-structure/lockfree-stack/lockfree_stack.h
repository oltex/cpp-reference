#pragma once
#include <utility>
#include <Windows.h>
#include <intrin.h>

namespace data_structure {
	template<typename type>
	class lockfree_stack final {
	private:
		using size_type = unsigned int;
		struct node final {
			node* _next;
			type _value;
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
		template<typename universal>
		inline void push(universal&& value) noexcept {
			node* current = reinterpret_cast<node*>(malloc(sizeof(node)));
			current->_value = std::forward<universal>(value);
			do
				current->_next = _head;
			while (current->_next != _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), current, current->_next));
		}
		inline auto pop(void) noexcept -> type {
			for (;;) {
				//auto current = _head;
				//if (current == _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), current->_next, current))
				//	return current->_value;

				auto current = _head;
				auto next = current->_next;
				if (current == _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head), next, current)) {
					type result = current->_value;
					delete current;
					return result;
				}
			}
		}
	public:
		//inline auto size(void) noexcept -> size_type {
		//}
		//inline auto empty(void) noexcept -> size_type {
		//}
	private:
		node* _head;
	};
}