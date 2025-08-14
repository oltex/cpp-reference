#pragma once
#include "../../intrusive/share_pointer.h"
#include "../my_class.h"
#include <iostream>
#include <memory>


namespace example::intrusive {
	class share_hook_class final : public my_class, public library::intrusive::share_pointer_hook<1> {
	public:
		using my_class::my_class;

		template<size_t index>
		inline void destructor(void) noexcept;
		template<>
		inline void destructor<1>(void) noexcept {
			delete this;
		}
	};
	inline void share_pointer(void) noexcept {
		share_hook_class* myclass = new share_hook_class(1);
		library::intrusive::share_pointer<share_hook_class, 1> shared_pointer(myclass);
	}
}