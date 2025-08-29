#pragma once
#include "../function.h"
#include "pool.h"
#include "vector.h"
#include <cassert>
#include <type_traits>

namespace library {
	template<typename type, auto predicate = library::less<type>, typename allocator = std::nullptr_t/*library::pool<type>*/, bool placement = true>
	class priority_queue final {
		using size_type = unsigned int;
		struct node final {
			node* _left, * _right, * _parent;
			type _value;
			inline explicit node(void) noexcept = delete;
			inline explicit node(node const&) noexcept = delete;
			inline explicit node(node&&) noexcept = delete;
			inline auto operator=(node const&) noexcept = delete;
			inline auto operator=(node&&) noexcept = delete;
			inline ~node(void) noexcept = delete;
		};
		size_type _size;
		node* _root;
		allocator::template rebind<node> _allocator;
	public:
		inline explicit priority_queue(void) noexcept
			: _size(0), _root(reinterpret_cast<node*>(library::allocate(sizeof(node*) * 3))) {
			//_last = _root->_left = _root->_right = _root->_parent = _root;
		};
		inline explicit priority_queue(priority_queue const&) noexcept = default;
		inline explicit priority_queue(priority_queue&&) noexcept = default;
		inline auto operator=(priority_queue const&) noexcept -> priority_queue & = default;
		inline auto operator=(priority_queue&&) noexcept -> priority_queue & = default;
		inline ~priority_queue(void) noexcept {
			library::deallocate(reinterpret_cast<void*>(_root));
		};

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			auto current = _allocator.allocate();
			if constexpr (true == placement)
				library::construct<type>(current->_value, std::forward<argument>(arg)...);

			auto mask = library::bit_mask_reverse(++_size);
			auto parent = _root;
			while (mask > 1) {
				if (_size & mask)
					parent = parent->_right;
				else
					parent = parent->_left;
				mask >>= 1;
			}
			current->_parent = parent;
			current->_left = current->_right = _root;
			if (_size & mask)
				parent->_right = current;
			else
				parent->_left = current;

			while (current->_parent != _root) {
				auto parent = current->_parent;
				if (predicate(parent->_value, current->_value))
					break;
				//swap(current, parent);
			}
		};
	};

	template<typename type, auto predicate, bool placement>
	class priority_queue<type, predicate, std::nullptr_t, placement> final {
		using size_type = unsigned int;
		vector<type, placement> _vector;
	public:
		inline explicit priority_queue(void) noexcept = default;
		inline explicit priority_queue(priority_queue const&) noexcept = default;
		inline explicit priority_queue(priority_queue&&) noexcept = default;
		inline auto operator=(priority_queue const&) noexcept -> priority_queue & = default;
		inline auto operator=(priority_queue&&) noexcept -> priority_queue & = default;
		inline ~priority_queue(void) noexcept = default;

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			_vector.emplace_back(std::forward<argument>(arg)...);
			auto leaf = _vector.back();
			auto child = _vector.size() - 1;
			while (0 < child) {
				auto parent = (child - 1) / 2;

				if (predicate(_vector[parent], leaf))
					break;
				_vector[child] = _vector[parent];
				child = parent;
			}
			_vector[child] = leaf;
		};
		inline void pop(void) noexcept {
			auto leaf = _vector.back();
			auto size = _vector.size() - 1;

			size_type parent = 0;
			for (;;) {
				auto left = parent * 2 + 1;
				if (size <= left)
					break;
				auto right = left + 1;

				if (size > right && predicate(_vector[right], _vector[left]))
					left = right;
				if (predicate(leaf, _vector[left]))
					break;

				_vector[parent] = _vector[left];
				parent = left;
			}
			_vector[parent] = leaf;
			_vector.pop_back();
		}
		inline auto top(void) const noexcept -> type& {
			return _vector.front();
		};
		inline void clear(void) noexcept {
			_vector.clear();
		}
		inline void swap(priority_queue& rhs) noexcept {
			library::swap(_vector, rhs._vector);
		}
		inline auto size(void) const noexcept -> size_type {
			return _vector.size();
		}
		inline bool empty(void) const noexcept {
			return _vector.empty();
		}
	};
}