#pragma once
#include <memory>
#include <Windows.h>

namespace data_structure::lockfree {
	template<typename type>
	class object_pool final {
	private:
		struct node final {
			node* _next;
			type _value;
		};
	public:
		inline explicit object_pool(void) noexcept
			: _head(0) {
		};
		inline explicit object_pool(object_pool const&) noexcept = delete;
		inline explicit object_pool(object_pool&& rhs) noexcept
			: _head(rhs._head) {
			rhs._head = nullptr;
		};
		inline auto operator=(object_pool const&) noexcept = delete;
		inline auto operator=(object_pool&& rhs) noexcept {
			_head = rhs._head;
			rhs._head = nullptr;
		}
		inline ~object_pool(void) noexcept {
			node* head = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & _head);
			while (nullptr != head) {
				node* next = head->_next;
				if constexpr (!std::is_trivially_destructible_v<type>)
					_head->_value.~type();
				free(head);
				head = next;
			}
		}
	public:
		template<typename... argument>
		inline auto acquire(argument&&... arg) noexcept -> type& {
			for (;;) {
				unsigned long long head = _head;
				node* current = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
				if (nullptr == current) {
					current = reinterpret_cast<node*>(malloc(sizeof(node)));
					if constexpr (std::is_class_v<type>) {
						if constexpr (std::is_constructible_v<type, argument...>)
							::new(reinterpret_cast<void*>(&current->_value)) type(std::forward<argument>(arg)...);
					}
					else if constexpr (1 == sizeof...(arg))
						current->_value = type(std::forward<argument>(arg)...);
					return current->_value;
				}
				unsigned long long next = reinterpret_cast<unsigned long long>(current->_next) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
					return current->_value;
			}
		}
		inline void release(type& value) noexcept {
			node* current = reinterpret_cast<node*>(reinterpret_cast<uintptr_t*>(&value) - 1);
			for (;;) {
				unsigned long long head = _head;
				current->_next = reinterpret_cast<node*>(head & 0x00007FFFFFFFFFFFULL);
				unsigned long long next = reinterpret_cast<unsigned long long>(current) + (head & 0xFFFF800000000000ULL)/* + 0x0000800000000000ULL*/;
				if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
					break;
			}
		}
	private:
		unsigned long long _head;
	};
}