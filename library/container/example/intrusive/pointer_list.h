#pragma once
#include "../../intrusive/pointer_list.h"
#include "../my_class.h"
#include <iostream>
#include <memory>


namespace example::intrusive {
	class pointer_class final : public my_class, public library::intrusive::pointer_hook<1>, public library::intrusive::list_hook<0> {
	public:
		using my_class::my_class;

		template<size_t index>
		inline static void deallocate(my_class* ptr) noexcept;
		template<>
		inline static void deallocate<1>(my_class* ptr) noexcept {
			delete ptr;
		}
	};
	inline void pointer_list(void) noexcept {
		library::intrusive::pointer_list<pointer_class, 0, 1> list;
		{
			library::intrusive::share_pointer<pointer_class, 1> pointer(new pointer_class(10));
			list.push_back(pointer);
		}
		list.erase(list.begin());
		//list.push_back()
	}
}