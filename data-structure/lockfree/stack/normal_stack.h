#pragma once
#include "../../../memory/memory.h"
#include "../../thread-local/pool/pool.h"

namespace library::data_structure {
	template<typename type>
	class stack {
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
		inline stack(void) noexcept
			: _head(nullptr) {
		}

		template<typename... argument>
		inline void push(argument&&... arg) noexcept {
			node* current = &_pool::instance().allocate();
			memory::construct<type>(current->_value, std::forward<argument>(arg)...);

			current->_next = _head;
			_head = current;
		}
		inline auto pop(void) noexcept -> type {
			node* current = _head;

			if (current == nullptr)
				__debugbreak();

			_head = current->_next;
			type result = current->_value;
			_pool::instance().deallocate(*current);
			return result;
		}
	private:
		alignas(64) node* _head;
	};
}