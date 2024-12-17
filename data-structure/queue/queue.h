#pragma once
#include "../memory-pool/memory_pool.h"

namespace data_structure {
	template<typename type, typename allocator = memory_pool<type>>
	class queue final {
	private:
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
		using rebind_allocator = allocator::template rebind<node>;
	public:
		inline explicit queue(void) noexcept = default;
		inline explicit queue(queue const& rhs) noexcept = delete;
		inline explicit queue(queue&& rhs) noexcept = delete;
		inline auto operator=(queue const& rhs) noexcept -> queue & = delete;
		inline auto operator=(queue&& rhs) noexcept -> queue & = delete;
		inline ~queue(void) noexcept = default;
	public:
		template<typename universal>
		inline void push(universal&& value) noexcept {
		
		}
		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {

		}
		inline void pop(void) noexcept {

		}
	public:
		inline auto front(void) noexcept -> type {
		}
		inline auto back(void) noexcept -> type {
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
	public:
		node* _head;
		node* _tail;
		rebind_allocator _allocator;
		size_type _size = 0;
	};
}