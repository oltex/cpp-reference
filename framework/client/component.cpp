#include "component.h"

namespace framework {
	void components::destory(library::rcu_pointer<component> pointer) noexcept {
		pointer.invalid([&](component* pointer) {
			auto& result = _pool.find(pointer->type())->_second;
			result->deallocate(pointer);
			});
	}
}