#pragma once
#include <utility>
#include <stdlib.h>
#include <malloc.h>

#include "../../algorithm/predicate/predicate.h"
#include "../../system/memory/memory.h"
#include "../pool/pool.h"
#include "../pair/pair.h"

namespace library::data_structure {
	template<typename type, auto predicate = algorithm::predicate::ordering<type>, typename allocator = pool<type>>
	class set final {
	private:
		using size_type = unsigned int;
		enum class color : bool { red, black };
		enum class direction : bool { left, right };
		struct node final {
			inline explicit node(void) noexcept = delete;
			inline explicit node(node const&) noexcept = delete;
			inline explicit node(node&&) noexcept = delete;
			inline auto operator=(node const&) noexcept = delete;
			inline auto operator=(node&&) noexcept = delete;
			inline ~node(void) noexcept = delete;
			node* _parent, * _left, * _right;
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
		inline explicit set(void) noexcept
			: _size(0) {
			_root = _nil = reinterpret_cast<node*>(system::memory::allocate(sizeof(node*) * 3 + sizeof(color) + sizeof(bool)));
#pragma warning(suppress: 6011)
			_nil->_parent = _nil->_left = _nil->_right = _nil;
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
		inline auto emplace(argument&&... arg) noexcept {
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
					auto result = predicate((*current)->_value, element->_value);
					if (std::strong_ordering::equal == result)
						return;
					else if (std::strong_ordering::less == result)
						current = &(*current)->_left;
					else
						current = &(*current)->_right;
				}
				element->_parent = parent;
				element->_left = element->_right = *current;
			}
			{
				node* current = element;
				while (_root != current) {
					node* parent = current->_parent;
					if (color::black == parent->_color)
						return;
					node* grand = parent->_parent;
					node* uncle = parent == grand->_left ? grand->_right : grand->_left;

					if (color::black == uncle->_color) {
						//restructure
						direction dir = parent == grand->_left ? direction::right : direction::left;
						if ((direction::left == dir && current == parent->_left) ||
							(direction::right == dir && current == parent->_right)) {
							rotate(parent,  static_cast<direction>(!static_cast<bool>(dir)));
							parent = current;
						}
						rotate(grand, dir);
						parent->_color = color::black;
						grand->_color = color::red;
						return;
					}
					parent->_color = color::black;
					grand->_color = color::red;
					uncle->_color = color::black;
					current = grand;
				}
				current->_color = color::black;
			}
			++_size;
		}

		inline void rotate(node* const current, direction const dir) noexcept {
			node* child = dir == direction::left ? current->_right : current->_left;

			if (current == _root) {
				_root = child;
				child->_parent = current->_parent;
			}
			else
				connect(current->_parent, child, current == current->_parent->_left ? direction::left : direction::right);

			switch (dir) {
			case direction::left:
				connect(current, child->_left, direction::right);
				break;
			case direction::right:
				connect(current, child->_right, direction::left);
				break;
			}
			connect(child, current, dir);
		}
		inline void connect(node* const parent, node* const child, direction const dir) const noexcept {
			switch (dir) {
			case direction::left:
				parent->_left = child;
				break;
			case direction::right:
				parent->_right = child;
				break;
			}
			child->_parent = parent;
		}
	private:
		node* _root;
		node* _nil;
		size_type _size;
		rebind_allocator _allocator;
	};
}