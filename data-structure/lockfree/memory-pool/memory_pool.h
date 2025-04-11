#pragma once
#include "../../../system-component/memory/memory.h"
#include <memory>
#include <Windows.h>

namespace library::data_structure::lockfree {
	template<typename type, bool placement = true, bool compress = true>
	class memory_pool final {
	private:
		union union_node final {
			inline explicit union_node(void) noexcept = delete;
			inline explicit union_node(union_node const&) noexcept = delete;
			inline explicit union_node(union_node&&) noexcept = delete;
			inline auto operator=(union_node const&) noexcept = delete;
			inline auto operator=(union_node&&) noexcept = delete;
			inline ~union_node(void) noexcept = delete;
			union_node* _next;
			type _value;
		};
		struct strcut_node {
			inline explicit strcut_node(void) noexcept = delete;
			inline explicit strcut_node(strcut_node const&) noexcept = delete;
			inline explicit strcut_node(strcut_node&&) noexcept = delete;
			inline auto operator=(strcut_node const&) noexcept = delete;
			inline auto operator=(strcut_node&&) noexcept = delete;
			inline ~strcut_node(void) noexcept = delete;
			strcut_node* _next;
			type _value;
		};
		using node = typename std::conditional<compress, union union_node, struct strcut_node>::type;
	public:
		inline explicit memory_pool(void) noexcept
			: _head(0) {
		};
		inline explicit memory_pool(memory_pool const&) noexcept = delete;
		inline explicit memory_pool(memory_pool&& rhs) noexcept
			: _head(rhs._head) {
			rhs._head = nullptr;
		};
		inline auto operator=(memory_pool const&) noexcept = delete;
		inline auto operator=(memory_pool&& rhs) noexcept {
			_head = rhs._head;
			rhs._head = nullptr;
		}
		inline ~memory_pool(void) noexcept {
			node* head = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & _head);
			while (nullptr != head) {
				node* next = head->_next;
				system_component::memory::deallocate<node>(head);
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
					current = system_component::memory::allocate<node>();
					break;
				}
				unsigned long long next = reinterpret_cast<unsigned long long>(current->_next) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
					break;
			}

			if constexpr (true == placement)
				system_component::memory::construct(current->_value, std::forward<argument>(arg)...);
			return current->_value;
		}
		inline void deallocate(type& value) noexcept {
			if constexpr (true == placement)
				system_component::memory::destruct(value);
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
		unsigned long long _head;
	};
}