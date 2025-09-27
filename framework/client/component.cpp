#include "component.h"

namespace framework {
	component::component(size_type const type_id) noexcept
		: _type_id(type_id) {
	};
	auto component::type_id(void) const noexcept -> size_type {
		return _type_id;
	}
	void components::deallocate_component(framework::component* component) noexcept {
		auto result = _component.find(component->type_id());
		result->_second->deallocate(component);
	}
}