#pragma once
#include <utility>
#include <stdlib.h>
#include <malloc.h>

#include "../../predicate/predicate.h"
#include "../../../system/memory/memory.h"
#include "../../../data-structure/pool/pool.h"

enum class color : bool { red, black };
enum direction : bool { left, right };

template<typename type, auto predicate = library::algorithm::predicate::ordering<type>, typename allocator = library::data_structure::pool<type>>
class set final {
public:
	using size_type = unsigned int;
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
		POINT _position;
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

		inline auto operator*(void) const noexcept -> type& {
			return _node->_value;
		}
		inline auto operator->(void) const noexcept -> type* {
			return &_node->_value;
		}
		inline auto operator++(void) noexcept -> iterator& {
			if (true == _node->_child[direction::right]->_nil) {
				while (false == _node->_parent->_nil && _node->_parent->_child[direction::right] == _node) {
					_node = _node->_parent;
					--_depth;
				}
				_node = _node->_parent;
				--_depth;
			}
			else {
				_node = _node->_child[direction::right];
				++_depth;
				while (false == _node->_child[direction::left]->_nil) {
					_node = _node->_child[direction::left];
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
		size_type _depth;
	};

	inline explicit set(void) noexcept
		: _size(0) {
		_root = _nil = reinterpret_cast<node*>(library::system::memory::allocate(sizeof(node*) * 3 + sizeof(color) + sizeof(bool)));
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
		library::system::memory::deallocate(reinterpret_cast<void*>(_root));
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
		library::system::memory::construct<type>(element->_value, std::forward<argument>(arg)...);
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
	inline auto begin(void) const noexcept -> iterator {
		node* current = _root;
		size_type depth = 0;
		while (false == current->_nil && false == current->_child[direction::left]->_nil) {
			current = current->_child[direction::left];
			++depth;
		}
		return iterator(current, depth);
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