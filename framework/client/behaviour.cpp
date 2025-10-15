#include "behaviour.h"

namespace framework {
	behaviour::behaviour(library::intrusive::share_pointer<object, 0>& parent) noexcept
		: _parent(parent) {
	}
}