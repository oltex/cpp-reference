#pragma once
#include "../../intrusive/pointer.h"
#include "../my_class.h"
#include <iostream>
#include <memory>


namespace example::intrusive {
	class share_hook_class final : public my_class, public library::intrusive::pointer_hook<1> {
	public:
		using my_class::my_class;

		/*template<size_t index>
		inline void destruct(void) noexcept;
		template<>
		inline void destruct<1>(void) noexcept {
			library::destruct(*this);
		}*/

		template<size_t index>
		inline static void deallocate(my_class* ptr) noexcept;
		template<>
		inline static void deallocate<1>(my_class* ptr) noexcept {
			delete ptr;
		}
	};
	inline void pointer(void) noexcept {
		std::shared_ptr<int> b(new int(10));
		std::weak_ptr<int> a(b);
		a.lock();
		share_hook_class* my_class = new share_hook_class(1);

		library::intrusive::share_pointer<share_hook_class, 1> shared_pointer(my_class);
		library::intrusive::weak_pointer<share_hook_class, 1> weak_pointer(shared_pointer);

		auto shared_pointer2 = weak_pointer.lock();

	}
}