#include "component.h"

namespace framework {
	component::component(void) noexcept 
		: _guid(library::create_guid()) {
	}
	auto component::guid(void) noexcept -> library::guid& {
		return _guid;
	}

	void components::destory(library::rcu_pointer<component> pointer) noexcept {
		pointer.invalid([&](component* pointer) {
			auto& result = _pool.find(pointer->type())->_second;
			result->deallocate(pointer);
			});
	}
}