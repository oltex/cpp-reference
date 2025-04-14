#pragma once
#include "../../../system-component/memory/memory.h"
#include "../pool/pool.h"
#include <utility>
#include <Windows.h>
#include <intrin.h>
#include <optional>

namespace library::data_structure::lockfree {
	template<typename type>
	class stack final {
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
		inline ~stack(void) noexcept {
			node* head = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & _head);
			while (nullptr != head) {
				node* next = head->_next;
				system_component::memory::destruct<type>(head->_value);
				_pool.deallocate(*head);
				head = next;
			}
		};
	public:
		template<typename... argument>
		inline void push(argument&&... arg) noexcept {
			node* current = &_pool.allocate();
			system_component::memory::construct<type>(current->_value, std::forward<argument>(arg)...);

			for (;;) {
				unsigned long long head = _head;
				current->_next = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
				unsigned long long next = reinterpret_cast<unsigned long long>(current) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
					break;
			}
		}
		inline auto pop(void) noexcept -> std::optional<type> {
			for (;;) {
				unsigned long long head = _head;
				node* address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
				if (nullptr == address)
					return std::nullopt;
				unsigned long long next = reinterpret_cast<unsigned long long>(address->_next) + (0xFFFF800000000000ULL & head) /*+ 0x0000800000000000ULL*/;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head)) {
					type result(std::move(address->_value));
					system_component::memory::destruct<type>(address->_value);
					_pool.deallocate(*address);
					return result;
				}
			}
		}
	private:
		unsigned long long _head;
		pool<node> _pool;
	};
}