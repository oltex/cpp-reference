#pragma once
#include "../function.h"
#include "pool.h"
#include <cassert>

namespace library {
	template<typename type, typename allocator = pool<type>, bool placement = true>
	class stack {
		using size_type = unsigned int;
		struct node final {
			node* _next;
			type _value;
			inline explicit node(void) noexcept = delete;
			inline explicit node(node const&) noexcept = delete;
			inline explicit node(node&&) noexcept = delete;
			inline auto operator=(node const&) noexcept = delete;
			inline auto operator=(node&&) noexcept = delete;
			inline ~node(void) noexcept = delete;
		};

		size_type _size;
		node* _head;
		allocator::template rebind<node> _allocator;
	public:
		inline explicit stack(void) noexcept
			: _size(0), _head(nullptr) {
		};
		inline explicit stack(stack const&) noexcept = delete;
		inline explicit stack(stack&& rhs) noexcept
			: _size(exchange(rhs._size, 0)), _head(exchange(rhs._head, nullptr)) {
		};
		inline auto operator=(stack const&) noexcept -> stack & = delete;
		inline auto operator=(stack&&) noexcept -> stack & = delete;
		inline ~stack(void) noexcept {
			clear();
		};

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			auto current = _allocator.allocate();
			if constexpr (true == placement)
				construct<type>(current->_value, std::forward<argument>(arg)...);
			current->_next = exchange(_head, current);
			++_size;
		}
		inline void pop(void) noexcept {
			assert(_size > 0 && "called on empty");
			if constexpr (true == placement)
				destruct<type>(_head->_value);
			_allocator.deallocate(exchange(_head, _head->_next));
			--_size;
		}
		inline auto top(void) noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return _head->_value;
		}

		inline void clear(void) noexcept {
			while (nullptr != _head) {
				if constexpr (true == placement)
					destruct<type>(_head->_value);
				_allocator.deallocate(exchange(_head, _head->_next));
			}
			_size = 0;
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto empty(void) const noexcept -> bool {
			return  0 == _size;
		}
	};
}