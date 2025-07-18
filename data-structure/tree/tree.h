#pragma once
#include <utility>
#include <stdlib.h>
#include <malloc.h>

#include "../../algorithm/predicate/predicate.h"
#include "../../system/memory/memory.h"
#include "../pool/pool.h"
#include "../pair/pair.h"

namespace library::data_structure {
	template<typename key_type, typename type, auto predicate = algorithm::predicate::ordering<key_type>, typename allocator = pool<type>>
	class map final {
	private:
		using size_type = unsigned int;
		using pair = pair<key_type, type>;
		enum color : bool { red, black };
		enum direction : bool { left, right };
		struct node final {
			inline explicit node(void) noexcept = delete;
			inline explicit node(node const&) noexcept = delete;
			inline explicit node(node&&) noexcept = delete;
			inline auto operator=(node const&) noexcept = delete;
			inline auto operator=(node&&) noexcept = delete;
			inline ~node(void) noexcept = delete;
			node* _parent, * _left, * _right;
			color _color = red;
			bool _nil = false;
			pair _pair;
		};
		using rebind_allocator = allocator::template rebind<node>;
		template <typename key_type, typename... argument>
		struct key_extract {
			static constexpr bool able = false;
		};
		template <typename key_type, typename type>
		struct key_extract<key_type, key_type, type> {
			static constexpr bool able = true;
			static auto execute(key_type const& key, type const& value) noexcept -> key_type const& {
				return key;
			}
		};

	public:
		class iterator final {
			friend map;
		public:
			inline explicit iterator(node* const node = nullptr) noexcept
				: _node(node) {
			}
			inline iterator(iterator const& rhs) noexcept
				: _node{ rhs._node } {
			}
			inline auto operator=(iterator const& rhs) noexcept -> iterator& {
				_node = rhs._node;
				return *this;
			}

			inline auto operator*(void) const noexcept -> pair& {
				return _node->_pair;
			}
			inline auto operator->(void) const noexcept -> pair* {
				return &_node->_pair;
			}
			inline auto operator++(void) noexcept -> iterator& {
				if (true == _node->_right->_nil) {
					while (false == _node->_parent->_nil && _node->_parent->_right == _node)
						_node = _node->_parent;
					_node = _node->_parent;
				}
				else {
					_node = _node->_right;
					while (false == _node->_left->_nil)
						_node = _node->_left;
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
		private:
			node* _node;
		};

		inline explicit map(void) noexcept
			: _size(0) {
			_root = _nil = reinterpret_cast<node*>(system::memory::allocate(sizeof(node*) * 3 + sizeof(color) + sizeof(bool)));
#pragma warning(suppress: 6011)
			_nil->_parent = _nil->_left = _nil->_right = _nil;
			_nil->_color = black;
			_nil->_nil = true;
		}
		inline explicit map(map const& rhs) noexcept;
		inline explicit map(map&& rhs) noexcept;
		inline auto operator=(map const& rhs) noexcept -> map&;
		inline auto operator=(map&& rhs) noexcept -> map&;
		inline ~map(void) noexcept {
			clear();
			system::memory::deallocate(reinterpret_cast<void*>(_root));
		}

		template<typename... argument>
		inline auto emplace2(argument&&... arg) noexcept {
			using key_extract = key_extract<key_type, std::remove_cvref_t<argument>...>;
			if constexpr (true == key_extract::able)
				auto& key = key_extract::execute(std::forward<argument>(arg)...);
			else {
				//auto current = &_allocator.allocate();
				//	system::memory::construct<type>(current->_value, std::forward<argument>(arg)...);
			}

			node* cur = _root;
		}

		template<typename... argument>
		inline auto emplace(key_type const& key, argument&&... arg) noexcept -> pair& {
			node** cur = &_root;
			node* parent = (*cur)->_parent;
			while (false == (*cur)->_nil) {
				parent = *cur;
				auto result = predicate((*cur)->_pair._first, key);
				if (std::strong_ordering::equal == result)
					return (*cur)->_pair;
				else if (std::strong_ordering::less == result)
					cur = &(*cur)->_left;
				else
					cur = &(*cur)->_right;
			}
			node* child = *cur;
			node* ret = *cur = reinterpret_cast<node*>(malloc(sizeof(node)));
			ret->_parent = parent;
			ret->_left = ret->_right = child;
			ret->_color = red;
			ret->_nil = false;
			new(&ret->_pair._first) key_type(key);
			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					::new(reinterpret_cast<void*>(&ret->_pair._second)) type(std::forward<argument>(arg)...);
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

			if constexpr (std::is_destructible_v<key_type> && !std::is_trivially_destructible_v<key_type>)
				cur->_pair._first.~key_type();
			if constexpr (std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
				cur->_pair._second.~type();
			free(cur);

			if (color_ == black)
				blance(res);
			--_size;
		}

		inline auto begin(void) const noexcept -> iterator {
			node* cur = _root;
			while (true != cur->_nil && true != cur->_left->_nil)
				cur = cur->_left;
			return iterator(cur);
		}
		inline auto end(void) const noexcept -> iterator {
			return iterator(_nil);
		}
		inline auto find(key_type const& key) const noexcept -> iterator {
			node* cur = _root;
			while (false == cur->_nil) {
				auto result = predicate(cur->_pair._first, key);
				if (std::strong_ordering::equal == result)
					return iterator(cur);
				else if (std::strong_ordering::less == result)
					cur = cur->_left;
				else
					cur = cur->_right;
			}
			return iterator(cur);
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
		inline void clear(void) noexcept {
			while (0 != _size)
				erase(iterator(_root));
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
	private:
		node* _root;
		node* _nil;
		size_type _size;
		rebind_allocator _allocator;
	};
}





#pragma once
#include <utility>
#include <stdlib.h>
#include <malloc.h>

#include "../../algorithm/predicate/predicate.h"
#include "../../system/memory/memory.h"
#include "../pool/pool.h"

namespace library::data_structure {
	template<typename type, auto predicate = algorithm::predicate::ordering<type>, typename allocator = pool<type>>
	class set final {
	private:
		using size_type = unsigned int;
		enum class color : bool { red, black };
		enum direction : bool { left, right };
		struct node final {
			inline explicit node(void) noexcept = delete;
			inline explicit node(node const&) noexcept = delete;
			inline explicit node(node&&) noexcept = delete;
			inline auto operator=(node const&) noexcept = delete;
			inline auto operator=(node&&) noexcept = delete;
			inline ~node(void) noexcept = delete;
			node* _parent;
			node* _child[2];
			color _color;
			bool _nil;
			type _value;
		};
		using rebind_allocator = allocator::template rebind<node>;
		template <typename type, typename... argument>
		struct extract {
			static constexpr bool able = false;
		};
		template <typename type>
		struct extract<type, type> {
			static constexpr bool able = true;
			static auto execute(type const& value) noexcept -> type const& {
				return value;
			}
		};
	public:
		class iterator final {
		public:
			inline explicit iterator(node* const node = nullptr) noexcept
				: _node(node) {
			}
			inline iterator(iterator const& rhs) noexcept
				: _node{ rhs._node } {
			}
			inline auto operator=(iterator const& rhs) noexcept -> iterator& {
				_node = rhs._node;
				return *this;
			}

			inline auto operator*(void) const noexcept -> type& {
				return _node->_value;
			}
			inline auto operator->(void) const noexcept -> type* {
				return &_node->_value;
			}
			inline auto operator++(void) noexcept -> iterator& {
				if (true == _node->_child[direction::right]->_nil) {
					while (false == _node->_parent->_nil && _node->_parent->_child[direction::right] == _node)
						_node = _node->_parent;
					_node = _node->_parent;
				}
				else {
					_node = _node->_child[direction::right];
					while (false == _node->_child[direction::left]->_nil)
						_node = _node->_child[direction::left];
				}
				return *this;
			}
			inline auto operator++(int) noexcept -> iterator {
				iterator iter(*this);
				operator++();
				return iter;
			}
			inline auto operator--(void) noexcept -> iterator& {
				if (true == _node->_child[direction::left]->_nil) {
					while (false == _node->_parent->_nil && _node->_parent->_child[direction::left] == _node)
						_node = _node->_parent;
					_node = _node->_parent;
				}
				else {
					_node = _node->_child[direction::left];
					while (true != _node->_child[direction::right]->_nil)
						_node = _node->_child[direction::right];
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
		};

		inline explicit set(void) noexcept
			: _size(0) {
			_root = _nil = reinterpret_cast<node*>(system::memory::allocate(sizeof(node*) * 3 + sizeof(color) + sizeof(bool)));
#pragma warning(suppress: 6011)
			_nil->_parent = _nil->_child[direction::left] = _nil->_child[direction::right] = _nil;
			_nil->_color = color::black;
			_nil->_nil = true;
		}
		inline explicit set(set const& rhs) noexcept;
		inline explicit set(set&& rhs) noexcept;
		inline auto operator=(set const& rhs) noexcept -> set&;
		inline auto operator=(set&& rhs) noexcept -> set&;
		inline ~set(void) noexcept {
			//clear();
			system::memory::deallocate(reinterpret_cast<void*>(_root));
		}

		template<typename... argument>
		inline auto emplace(argument&&... arg) noexcept -> iterator {
			//using extract = extract<type, std::remove_cvref_t<argument>...>;
			//type const* value;
			//if constexpr (true == extract::able)
			//	value = &extract::execute(std::forward<argument>(arg)...);
			//else {
			//	value = &current->_value;
			//}

			auto element = &_allocator.allocate();
			system::memory::construct<type>(element->_value, std::forward<argument>(arg)...);
			element->_color = color::red;
			element->_nil = false;

			{
				node** current = &_root;
				node* parent = _nil;
				while (false == (*current)->_nil) {
					parent = *current;
					auto result = predicate(element->_value, (*current)->_value);
					if (std::strong_ordering::equal == result)
						return iterator(*current);
					else if (std::strong_ordering::less == result)
						current = &(*current)->_child[direction::left];
					else
						current = &(*current)->_child[direction::right];
				}
				element->_parent = parent;
				element->_child[direction::left] = element->_child[direction::right] = *current;
				*current = element;
			}
			{
				node* current = element;
				for (;;) {
					if (_root == current) {
						current->_color = color::black;
						break;
					}
					node* parent = current->_parent;
					if (color::black == parent->_color)
						break;
					node* grand = parent->_parent;
					node* uncle = parent == grand->_child[direction::left] ? grand->_child[direction::right] : grand->_child[direction::left];

					if (color::black == uncle->_color) {
						//restructure
						direction dir = parent == grand->_child[direction::left] ? direction::right : direction::left;
						if (current == parent->_child[dir]) {
							rotate(parent, static_cast<direction>(!static_cast<bool>(dir)));
							parent = current;
						}
						rotate(grand, dir);
						parent->_color = color::black;
						grand->_color = color::red;
						break;
					}
					parent->_color = color::black;
					grand->_color = color::red;
					uncle->_color = color::black;
					current = grand;
				}
			}
			++_size;
			return iterator(element);
		}
		inline auto erase(iterator const& iter) noexcept {
			node* element = iter.node;
			if (true == element->_nil)
				return;

			node* current = element->_child[direction::left];
		}

		inline auto begin(void) const noexcept -> iterator {
			node* current = _root;
			while (false == current->_nil && false == current->_child[direction::left]->_nil)
				current = current->_child[direction::left];
			return iterator(current);
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
	private:
		inline void rotate(node* const current, direction const dir) noexcept {
			node* child = current->_child[!dir];

			if (current == _root) {
				_root = child;
				child->_parent = current->_parent;
			}
			else
				connect(current->_parent, child, current == current->_parent->_child[direction::left] ? direction::left : direction::right);

			connect(current, child->_child[dir], static_cast<direction>(!static_cast<bool>(dir)));
			connect(child, current, dir);
		}
		inline void connect(node* const parent, node* const child, direction const dir) const noexcept {
			parent->_child[dir] = child;
			child->_parent = parent;
		}
		node* _root;
		node* _nil;
		size_type _size;
		rebind_allocator _allocator;
	};
}