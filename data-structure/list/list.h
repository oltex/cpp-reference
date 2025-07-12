#pragma once
#include <utility>
#include <stdlib.h>
#include <malloc.h>
#include <cassert>
#include "../pool/pool.h"
#include "../../memory/memory.h"
#include "../../algorithm/swap/swap.h"
#include "../../algorithm/exchange/exchange.h"

namespace library {
	template<typename type, typename allocator = pool<type>, bool placement = true>
	class list final {
		using size_type = unsigned int;
		struct node final {
			node* _prev, * _next;
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
		class iterator final {
		public:
			node* _node;

			inline explicit iterator(node* const node = nullptr) noexcept
				: _node(node) {
			}
			inline iterator(iterator const&) noexcept = default;
			inline explicit iterator(iterator&&) noexcept = default;
			inline auto operator=(iterator const&) noexcept -> iterator & = default;
			inline auto operator=(iterator&&) noexcept -> iterator & = default;
			inline ~iterator() noexcept = default;

			inline auto operator*(void) const noexcept -> type& {
				return _node->_value;
			}
			inline auto operator->(void) const noexcept -> type* {
				return &_node->_value;
			}
			inline auto operator++(void) noexcept -> iterator& {
				_node = _node->_next;
				return *this;
			}
			inline auto operator++(int) noexcept -> iterator {
				iterator iter(*this);
				_node = _node->_next;
				return iter;
			}
			inline auto operator--(void) noexcept -> iterator& {
				_node = _node->_prev;
				return *this;
			}
			inline auto operator--(int) noexcept -> iterator {
				iterator iter(*this);
				_node = _node->_prev;
				return iter;
			}
			inline bool operator==(iterator const& rhs) const noexcept {
				return _node == rhs._node;
			}
			inline bool operator!=(iterator const& rhs) const noexcept {
				return _node != rhs._node;
			}
		};

		inline explicit list(void) noexcept
			: _size(0), _head(reinterpret_cast<node*>(allocate(sizeof(node*) * 2))) {
#pragma warning(suppress: 6011)
			_head->_next = _head->_prev = _head;
		}
		inline explicit list(std::initializer_list<type> init_list) noexcept
			: list() {
			for (auto& iter : init_list)
				emplace_back(iter);
		}
		inline explicit list(iterator const& begin, iterator const& end) noexcept
			: list() {
			for (auto iter = begin; iter != end; ++iter)
				emplace_back(*iter);
		}
		inline list(list const& rhs) noexcept
			: list(rhs.begin(), rhs.end()) {
		}
		inline explicit list(list&& rhs) noexcept
			: list() {
			swap(rhs);
		}
		inline auto operator=(list const& rhs) noexcept -> list& {
			assert(this != &rhs && "self-assignment");
			list(rhs).swap(*this);
			return *this;
		};
		inline auto operator=(list&& rhs) noexcept -> list& {
			assert(this != &rhs && "self-assignment");
			list(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~list(void) noexcept {
			clear();
			deallocate(reinterpret_cast<void*>(_head));
		}

		template<typename... argument>
		inline auto emplace_front(argument&&... arg) noexcept -> type& {
			return *emplace(begin(), std::forward<argument>(arg)...);
		}
		template<typename... argument>
		inline auto emplace_back(argument&&... arg) noexcept -> type& {
			return *emplace(end(), std::forward<argument>(arg)...);
		}
		template<typename... argument>
		inline auto emplace(iterator const& iter, argument&&... arg) noexcept -> iterator {
			auto current = _allocator.allocate();
			if constexpr (true == placement)
				construct<type>(current->_value, std::forward<argument>(arg)...);
			auto next = iter._node;
			auto prev = next->_prev;

			prev->_next = current;
			current->_prev = prev;
			current->_next = next;
			next->_prev = current;

			++_size;
			return iterator(current);
		}
		inline void pop_front(void) noexcept {
			erase(begin());
		}
		inline void pop_back(void) noexcept {
			erase(--end());
		}
		inline auto erase(iterator const& iter) noexcept -> iterator {
			assert(_size > 0 && "called on empty");
			assert(iter._node != _head && "erase on sentinel");
			auto current = iter._node;
			auto prev = current->_prev;
			auto next = current->_next;

			prev->_next = next;
			next->_prev = prev;

			if constexpr (true == placement)
				destruct<type>(current->_value);
			_allocator.deallocate(current);
			--_size;
			return iterator(next);
		}

		inline auto front(void) const noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return _head->_next->_value;
		}
		inline auto back(void) const noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return _head->_prev->_value;
		}
		inline auto begin(void) const noexcept -> iterator {
			return iterator(_head->_next);
		}
		inline auto end(void) const noexcept -> iterator {
			return iterator(_head);
		}
		inline void swap(list& rhs) noexcept {
			library::swap(_head, rhs._head);
			library::swap(_size, rhs._size);
		}
		inline void clear(void) noexcept {
			auto current = _head->_next;
			while (current != _head) {
				if constexpr (true == placement)
					destruct<type>(current->_value);
				_allocator.deallocate(exchange(current, current->_next));
			}
			_head->_next = _head->_prev = _head;
			_size = 0;
		}
		inline void splice(iterator const& _before, iterator const& _first, iterator const& _last) noexcept {
			node* before = _before._node;
			node* first = _first._node;
			node* last = _last._node;

			node* first_prev = first->_prev;
			first_prev->_next = last;
			node* last_prev = last->_prev;
			last_prev->_next = before;
			node* before_prev = before->_prev;
			before_prev->_next = first;

			before->_prev = last_prev;
			last->_prev = first_prev;
			first->_prev = before_prev;

			//return last;
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
	};
}