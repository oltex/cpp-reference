#include "component.h"

namespace framework {
	void components::deallocate_component(framework::component* component) noexcept {
		auto result = _component.find(component->name());
		result->_second->deallocate(component);
	}
}