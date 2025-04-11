#pragma once
#include "../../system-component/memory/memory.h"

namespace library::data_structure {
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
		template <typename other>
		using rebind = memory_pool<other>;
	public:
		inline explicit memory_pool(void) noexcept = default;
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
			while (nullptr != _head) {
#pragma warning(suppress: 6001)
				node* next = _head->_next;
				system_component::memory::deallocate(_head);
				_head = next;
			}
		}
	public:
		template<typename... argument>
		inline auto allocate(argument&&... arg) noexcept -> type& {
			node* current;
			if (nullptr == _head)
				current = system_component::memory::allocate<node>();
			else {
				current = _head;
				_head = current->_next;
			}
			if constexpr (true == placement)
				system_component::memory::construct(current->_value, std::forward<argument>(arg)...);
			return current->_value;
		}
		inline void deallocate(type& value) noexcept {
			if constexpr (true == placement)
				system_component::memory::destruct(value);
			node* current = reinterpret_cast<node*>(reinterpret_cast<unsigned char*>(&value) - offsetof(node, _value));
			current->_next = _head;
			_head = current;
		}
	public:
		inline void swap(memory_pool& rhs) noexcept {
			auto head = _head;
			_head = rhs._head;
			rhs._head = head;
		}
	private:
		node* _head = nullptr;
	};
}