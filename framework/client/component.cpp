#include "component.h"

namespace framework {
	component::component(size_type const type_id) noexcept
		: _type_id(type_id) {
	};

	auto component::type_id(void) const noexcept -> size_type {
		return _type_id;
	}
}