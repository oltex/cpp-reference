#pragma once
#include <memory>
#ifdef debug
#include <intrin.h> 
#endif

namespace data_structure {
	template<typename type>
	class object_pool final {
	private:
		struct node final {
			node* _next;
			type _value;
		};
	public:
		inline explicit object_pool(void) noexcept = default;
		inline explicit object_pool(object_pool const&) noexcept = delete;
		inline explicit object_pool(object_pool&& rhs) noexcept
			: _head(rhs._head) {
			rhs._head = nullptr;
		};
		inline auto operator=(object_pool const&) noexcept = delete;
		inline auto operator=(object_pool&& rhs) noexcept {
			_head = rhs._head;
			rhs._head = nullptr;
		}
		inline ~object_pool(void) noexcept {
			while (nullptr != _head) {
#pragma warning(suppress: 6001)
				node* next = _head->_next;
				if constexpr (std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
					_head->_value.~type();
				free(_head);
				_head = next;
			}
		}
	public:
		template<typename... argument>
		inline auto allocate(argument&&... arg) noexcept -> type& {
			auto current = reinterpret_cast<node*>(malloc(sizeof(node)));
#ifdef debug
			current->_next = reinterpret_cast<node*>(this);
#endif
			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					::new(reinterpret_cast<void*>(&current->_value)) type(std::forward<argument>(arg)...);
			}
			else if constexpr (1 == sizeof...(arg))
#pragma warning(suppress: 6011)
				current->_value = type(std::forward<argument>(arg)...);
#pragma warning(suppress: 6011)
			return current->_value;
		}
	public:
		inline auto acquire(void) noexcept -> type& {
			node* current = _head;
#ifdef debug
			current->_next = reinterpret_cast<node*>(this);
#endif
			_head = current->_next;
			return current->_value;
		}
		inline void release(type& value) noexcept {
			node* current = reinterpret_cast<node*>(reinterpret_cast<uintptr_t*>(&value) - 1);
#ifdef debug
			if (reinterpret_cast<node*>(this) != current->_next)
				__debugbreak();
#endif
			current->_next = _head;
			_head = current;
		}
	public:
		inline bool empty(void) const noexcept {
			return nullptr == _head;
		}
	private:
		node* _head = nullptr;
	};
}