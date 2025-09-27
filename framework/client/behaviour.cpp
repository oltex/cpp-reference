#include "behaviour.h"

namespace framework {
	behaviour::behaviour(size_type const type_id) noexcept
		: component(type_id) {
	}
}