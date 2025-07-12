#pragma once
#include "../pool/pool.h"

namespace library::data_structure {
	template<typename type, typename allocator = pool<type>, bool placement = true>
	class queue final {
		using size_type = unsigned int;
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
		size_type _size = 0;
		node* _head;
		node* _tail;
		allocator::template rebind<node> _allocator;
	public:
		inline explicit queue(void) noexcept {
			node* current = &_allocator.allocate();
			current->_next = nullptr;
			_head = _tail = current;
		};
		inline explicit queue(queue const& rhs) noexcept = delete;
		inline explicit queue(queue&& rhs) noexcept = delete;
		inline auto operator=(queue const& rhs) noexcept -> queue & = delete;
		inline auto operator=(queue&& rhs) noexcept -> queue & = delete;
		inline ~queue(void) noexcept {
			node* current = _head;
			node* next = current->_next;
			while (nullptr != next) {
				_allocator.deallocate(*current);
				if constexpr (true == placement)
					system::memory::destruct(next->_value);
				current = next;
				next = current->_next;
			}
			_allocator.deallocate(*current);
		};

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			node* current = &_allocator.allocate();
			if constexpr (true == placement)
				system::memory::construct<type>(current->_value, std::forward<argument>(arg)...);

			current->_next = nullptr;
			_tail->_next = current;
			_tail = current;
			++_size;
		}
		inline void pop(void) noexcept {
			node* current = _head;
			_head = current->_next;
			_allocator.deallocate(*current);
			if constexpr (true == placement)
				system::memory::destruct(_head->_value);
			--_size;
		}

		inline auto front(void) noexcept -> type& {
			return _head->_next->_value;
		}
		inline auto back(void) noexcept -> type& {
			return _tail->_value;
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto empty(void) const noexcept -> bool {
			return 0 == _size;
		}
	};
}