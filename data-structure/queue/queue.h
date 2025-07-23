#pragma once
#include <cassert>
#include <memory>
#include <type_traits>
#include "../pool/pool.h"
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace library {
	template<typename type, typename allocator = pool<type>, bool placement = true>
	class queue final {
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
		size_type _size = 0;
		node* _head;
		node* _tail;
		allocator::template rebind<node> _allocator;
	public:
		inline explicit queue(void) noexcept {
			auto current = _allocator.allocate();
			current->_next = nullptr;
			_head = _tail = current;
		};
		inline explicit queue(queue const&) noexcept = delete;
		inline explicit queue(queue&& rhs) noexcept
			: queue() {
			swap(rhs);
		};
		inline auto operator=(queue const&) noexcept -> queue & = delete;
		inline auto operator=(queue&& rhs) noexcept -> queue& {
			assert(this != &rhs && "self-assignment");
			queue(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~queue(void) noexcept {
			auto current = _head->_next;
			while (nullptr != current) {
				if constexpr (true == placement)
					library::destruct(current->_value);
				_allocator.deallocate(exchange(current, current->_next));
			}
			deallocate(reinterpret_cast<void*>(_head));
		};

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			auto current = _allocator.allocate();
			current->_next = nullptr;
			if constexpr (true == placement)
				library::construct<type>(current->_value, std::forward<argument>(arg)...);
			library::exchange(_tail, current)->_next = current;
			++_size;
		}
		inline void pop(void) noexcept {
			assert(_size > 0 && "called on empty");
			auto current = library::exchange(_head->_next, _head->_next->_next);
			if (_tail == current)
				_tail = _head;
			if constexpr (true == placement)
				library::destruct(current->_value);
			_allocator.deallocate(current);
			--_size;
		}
		inline auto front(void) noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return _head->_next->_value;
		}
		inline auto back(void) noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return _tail->_value;
		}
		inline void swap(queue& rhs) noexcept {
			library::swap(_size, rhs._size);
			library::swap(_head, rhs._head);
			library::swap(_tail, rhs._tail);
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto empty(void) const noexcept -> bool {
			return 0 == _size;
		}
		inline void clear(void) noexcept {
			while (0 != _size)
				pop();
		}
	};
}