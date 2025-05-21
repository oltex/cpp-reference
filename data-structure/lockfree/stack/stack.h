#pragma once
#include "../../../system/memory/memory.h"
#include "../../thread-local/pool/pool.h"
#include <utility>
#include <Windows.h>
#include <intrin.h>
#include <optional>

namespace library::data_structure::lockfree {
	template<typename type>
	class stack final {
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
		using _pool = _thread_local::pool<node>;
	public:
		inline explicit stack(void) noexcept
			: _head(0) {
		}
		inline explicit stack(stack const&) noexcept = delete;
		inline explicit stack(stack&&) noexcept = delete;
		inline auto operator=(stack const&) noexcept -> stack & = delete;
		inline auto operator=(stack&&) noexcept -> stack & = delete;
		inline ~stack(void) noexcept {
			node* head = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & _head);
			while (nullptr != head) {
				node* next = head->_next;
				system::memory::destruct<type>(head->_value);
				_pool::instance().deallocate(*head);
				head = next;
			}
		};
	public:
		template<typename... argument>
		inline void push(argument&&... arg) noexcept {
			node* current = &_pool::instance().allocate();
			system::memory::construct<type>(current->_value, std::forward<argument>(arg)...);

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
				unsigned long long next = reinterpret_cast<unsigned long long>(address->_next) + (0xFFFF800000000000ULL & head);
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head)) {
					type result(std::move(address->_value));
					_pool::instance().deallocate(*address);
					return result;
				}
			}
		}
	private:
		alignas(64) unsigned long long _head;
	};
}