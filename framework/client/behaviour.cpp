#include "behaviour.h"

namespace framework {
	behaviour::behaviour(size_type const type_id, library::intrusive::share_pointer<object, 0>& parent) noexcept
		: component(type_id), _parent(parent) {
	}
}