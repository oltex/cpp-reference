#pragma once
#include "list.h"
#include "pointer.h"

namespace library::intrusive {
	template<typename type, size_t list_index, size_t pointer_index>
	class pointer_list : public list<type, list_index> {
		using base = list<type, list_index>;
		using pointer = share_pointer<type, pointer_index>;
	public:
		using iterator = base::iterator;

		inline explicit pointer_list(void) noexcept = default;
		inline explicit pointer_list(pointer_list const&) noexcept;
		inline explicit pointer_list(pointer_list&&) noexcept;
		inline auto operator=(pointer_list const&) noexcept -> pointer_list&;
		inline auto operator=(pointer_list&&) noexcept -> pointer_list&;
		inline ~pointer_list(void) noexcept {
			for (auto begin = base::begin(), end = base::end(); begin != end;) {
				pointer ptr;
				ptr.set(&*begin);
				begin = base::erase(begin);
			}
		};

		inline void push_front(pointer& ptr) noexcept {
			insert(base::begind(), ptr);
		}
		inline void push_back(pointer& ptr) noexcept {
			insert(base::end(), ptr);
		}
		inline static void insert(iterator iter, pointer ptr) noexcept {
			base::insert(iter, *ptr);
			ptr.reset();
		}
		inline void pop_front(void) noexcept {
			erase(base::begin());
		}
		inline void pop_back(void) noexcept {
			erase(--base::end());
		}
		inline static auto erase(iterator const& iter) noexcept -> iterator {
			pointer ptr;
			ptr.set(&*iter);
			return base::erase(iter);
		}
		inline static auto erase(pointer& ptr) noexcept -> iterator {
			pointer _ptr;
			_ptr.set(&*ptr);
			return base::erase(*ptr);
		}
	};
}