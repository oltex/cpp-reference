#pragma once
#include "../../memory/memory.h"
#include "../../algorithm/exchange/exchange.h"

namespace library {
	template<typename type, bool placement = true, bool compress = true>
	class pool final {
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
		struct strcut_node final {
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
		template <typename other>
		using rebind = pool<other>;
		node* _head;
	public:
		inline explicit pool(void) noexcept 
			: _head(nullptr) {
		};
		inline explicit pool(pool const&) noexcept = delete;
		inline explicit pool(pool&& rhs) noexcept
			: _head(exchange(rhs._head, nullptr)) {
		};
		inline auto operator=(pool const&) noexcept = delete;
		inline auto operator=(pool&& rhs) noexcept -> pool& {
			while (nullptr != _head) {
#pragma warning(suppress: 6001)
				auto next = _head->_next;
				library::deallocate<node>(_head);
				_head = next;
			}
			_head = exchange(rhs._head, nullptr);
			return *this;
		}
		inline ~pool(void) noexcept {
			while (nullptr != _head)
#pragma warning(suppress: 6001)
				library::deallocate<node>(exchange(_head, _head->_next));
		}

		template<typename... argument>
		inline auto allocate(argument&&... arg) noexcept -> type* {
			node* current;
			if (nullptr == _head)
				current = library::allocate<node>();
			else {
				current = _head;
				_head = current->_next;
			}
			if constexpr (true == placement)
				library::construct<type, argument...>(current->_value, std::forward<argument>(arg)...);
			return &current->_value;
		}
		inline void deallocate(type* const value) noexcept {
			if constexpr (true == placement)
				library::destruct<type>(*value);
			auto current = reinterpret_cast<node*>(reinterpret_cast<unsigned char*>(value) - offsetof(node, _value));
			current->_next = _head;
			_head = current;
		}
	};
}