#include "object.h"

namespace framework {
	object::object(object_share_ptr& parent) noexcept
		: _parent(parent) {

	}

}