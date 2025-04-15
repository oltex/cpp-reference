#pragma once
#include <utility>
#include <stdlib.h>
#include <malloc.h>

#include "predicate.h"
#include "pair.h"

namespace library::data_structure {
	template<typename key_type, typename type, typename predicate = less<key_type>>
	class map final {
	public:
		using size_type = unsigned int;
		using pair = pair<key_type, type>;
		enum color : bool { red, black };
		enum direction : bool { left, right };
		struct node final {
			node* _parent, * _left, * _right;
			color _color = red;
			bool _nil = false;
			pair _pair;
			POINT _position;
		};
	public:
		class iterator final {
			friend map;
		public:
			inline explicit iterator(node* const node = nullptr) noexcept
				: _node(node) {
			}
			inline explicit iterator(node* node, size_type const depth) noexcept
				: _node(node), _depth(depth) {
			}
			inline iterator(iterator const& rhs) noexcept
				: _node{ rhs._node } {
			}
			inline auto operator=(iterator const& rhs) noexcept -> iterator& {
				_node = rhs._node;
				return *this;
			}
		public:
			inline auto operator*(void) const noexcept -> pair& {
				return _node->_pair;
			}
			inline auto operator++(void) noexcept -> iterator& {
				if (true == _node->_right->_nil) {
					while (false == _node->_parent->_nil && _node->_parent->_right == _node) {
						_node = _node->_parent;
						--_depth;
					}
					_node = _node->_parent;
					--_depth;
				}
				else {
					_node = _node->_right;
					++_depth;
					while (true != _node->_left->_nil) {
						_node = _node->_left;
						++_depth;
					}
				}
				return *this;
			}
			inline auto operator++(int) noexcept -> iterator {
				iterator iter(*this);
				operator++();
				return iter;
			}
			inline auto operator--(void) noexcept -> iterator& {
				if (true == _node->_left->_nil) {
					while (false == _node->_parent->_nil && _node->_parent->_left == _node)
						_node = _node->_parent;
					_node = _node->_parent;
				}
				else {
					_node = _node->_left;
					while (true != _node->_right->_nil)
						_node = _node->_right;
				}
				return *this;
			}
			inline auto operator--(int) noexcept -> iterator {
				iterator iter(*this);
				operator--();
				return iter;
			}
			inline bool operator==(iterator const& rhs) const noexcept {
				return _node == rhs._node;
			}
			inline bool operator!=(iterator const& rhs) const noexcept {
				return _node != rhs._node;
			}
		public:
			node* _node;
			size_type _depth;
		};
	public:
		inline explicit map(void) noexcept {
			_root = _nil = static_cast<node*>(calloc(1, (sizeof(node*) * 3) + sizeof(color) + sizeof(bool)));
			_nil->_nil = true;
			_nil->_parent = _nil->_left = _nil->_right = _nil;
			_nil->_color = black;
		}
		//not implemented
		inline explicit map(map const& rhs) noexcept;
		//not implemented
		inline explicit map(map&& rhs) noexcept;
		//not implemented
		inline auto operator=(map const& rhs) noexcept -> map&;
		//not implemented
		inline auto operator=(map&& rhs) noexcept -> map&;
		inline ~map(void) noexcept {
			clear();
			free(_nil);
		}
	public:
		template<typename universal>
		inline void insert(key_type const& key, universal&& value) const noexcept {
			emplace(key, std::forward<universal>(value));
		}
		template<typename... argument>
		inline auto emplace(key_type const& key, argument&&... arg) noexcept -> pair& {
			node** cur = &_root;
			node* parent = (*cur)->_parent;
			while (false == (*cur)->_nil) {
				parent = *cur;
				std::strong_ordering result = _predicate(key, (*cur)->_pair._first);
				if (std::strong_ordering::equal == result)
					return (*cur)->_pair;
				else if (std::strong_ordering::less == result)
					cur = &(*cur)->_left;
				else if (std::strong_ordering::greater == result)
					cur = &(*cur)->_right;
			}
			node* child = *cur;
			node* ret = *cur = static_cast<node*>(malloc(sizeof(node)));
			ret->_parent = parent;
			ret->_left = ret->_right = child;
			ret->_color = red;
			ret->_nil = false;
			new(&ret->_pair._first) key_type(key);
			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					new(&ret->_pair._second) type(std::forward<argument>(arg)...);
			}
			else if constexpr (1 == sizeof...(arg))
				ret->_pair._second = type(std::forward<argument>(arg)...);

			recolor(ret);
			++_size;
			return ret->_pair;
		}
		inline void erase(key_type const& key) noexcept {
			iterator iter = find(key);
			erase(iter);
		}
		inline void erase(iterator const& iter) noexcept {
			node* cur = iter._node;
			if (true == cur->_nil)
				return;
			node* child = cur->_right;
			node* res = child;

			if (false == cur->_right->_nil) {
				res = child->_right;
				res->_parent = child;
			}
			do {
				if (true == cur->_left->_nil)
					break;
				child = cur->_left;
				res = child->_left;
				res->_parent = child;
				if (true == cur->_right->_nil)
					break;

				if (true != child->_right->_nil) {
					while (true != child->_right->_nil)
						child = child->_right;
					res = child->_left;
					connect(child->_parent, child->_left, right);
					connect(child, cur->_left, left);
				}
				connect(child, cur->_right, right);
			} while (false);

			if (_root == cur) {
				_root = child;
				child->_parent = cur->_parent;
			}
			else
				connect(cur->_parent, child, cur == cur->_parent->_left ? left : right);

			color color_ = child->_color;
			child->_color = cur->_color;

			if constexpr (std::is_destructible_v<key_type>)
				cur->_pair._first.~key_type();
			if constexpr (std::is_destructible_v<type>)
				cur->_pair._second.~type();
			free(cur);

			if (color_ == black)
				blance(res);
			--_size;
		}
	public:
		inline auto begin(void) const noexcept -> iterator {
			node* cur = _root;
			size_type depth = 0;
			while (true != cur->_nil && true != cur->_left->_nil) {
				cur = cur->_left;
				++depth;
			}
			return iterator(cur, depth);
		}
		inline auto end(void) const noexcept -> iterator {
			return iterator(_nil);
		}
		inline auto find(key_type const& key) const noexcept -> iterator {
			node* cur = _root;
			while (false == cur->_nil) {
				std::strong_ordering result = _predicate(key, cur->_pair._first);
				if (std::strong_ordering::equal == result)
					return iterator(cur);
				else if (std::strong_ordering::less == result)
					cur = cur->_left;
				else if (std::strong_ordering::greater == result)
					cur = cur->_right;
			}
			return iterator(cur);
		}
	public:
		inline void clear(void) noexcept {
			while (0 != _size)
				erase(iterator(_root));
		}
	public:
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
	private:
		inline void connect(node* const parent, node* const child, direction const dir) const noexcept {
			switch (dir) {
			case left:
				parent->_left = child;
				break;
			case right:
				parent->_right = child;
				break;
			}
			child->_parent = parent;
		}
		inline void rotate(node* const cur, direction const dir) noexcept {
			node* next = dir == left ? cur->_right : cur->_left;

			if (cur == _root) {
				_root = next;
				next->_parent = cur->_parent;
			}
			else
				connect(cur->_parent, next, cur == cur->_parent->_left ? left : right);

			switch (dir) {
			case left:
				connect(cur, next->_left, right);
				break;
			case right:
				connect(cur, next->_right, left);
				break;
			}
			connect(next, cur, dir);
		}
		inline void recolor(node* cur) noexcept {
			while (_root != cur) {
				node* parent = cur->_parent;
				if (black == parent->_color)
					return;
				node* grand = parent->_parent;
				node* uncle = parent == grand->_left ? grand->_right : grand->_left;

				if (black == uncle->_color) {
					restructure(cur, parent, grand);
					return;
				}
				parent->_color = black;
				grand->_color = red;
				uncle->_color = black;
				cur = grand;
			}
			cur->_color = black;
		}
		inline void restructure(node* cur, node* parent, node* grand) noexcept {
			direction dir = parent == grand->_left ? right : left;
			switch (dir) {
			case left:
				if (cur == parent->_left) {
					rotate(parent, right);
					parent = cur;
				}
				break;
			case right:
				if (cur == parent->_right) {
					rotate(parent, left);
					parent = cur;
				}
				break;
			}
			rotate(grand, dir);
			parent->_color = black;
			grand->_color = red;
		}
		inline void blance(node* cur) {
			while (_root != cur) {
				if (red == cur->_color) {
					cur->_color = black;
					return;
				}
				node* parent = cur->_parent;
				direction dir = cur == parent->_left ? left : right;
				node* sibling = dir == left ? parent->_right : parent->_left;

				if (red == sibling->_color) {
					sibling->_color = black;
					parent->_color = red;
					rotate(parent, dir);
					continue;
				}
				node* nibling_close = dir == left ? sibling->_left : sibling->_right;
				node* nibling_far = dir == left ? sibling->_right : sibling->_left;

				if (nibling_far->_color == red) {
					sibling->_color = parent->_color;
					parent->_color = black;
					nibling_far->_color = black;
					rotate(parent, dir);
					return;
				}
				if (nibling_close->_color == red) {
					nibling_close->_color = black;
					sibling->_color = red;
					rotate(sibling, (direction)!(bool)dir);
					cur->_parent = parent;
					continue;
				}
				sibling->_color = red;
				cur = parent;
			}
		}
	public:
		size_type debug(node* parent) noexcept {
			if (parent->_nil)
				return 0;
			int left = debug(parent->_left);
			int right = debug(parent->_right);
			if (left != right)
				int a = 10;

			if (parent->_color == black)
				left++;
			return left;
		}
	public:
		node* _root;
		node* _nil;
		size_type _size = 0;
		inline static predicate const _predicate;
	};
}