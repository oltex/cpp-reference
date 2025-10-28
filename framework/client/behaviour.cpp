#include "behaviour.h"

namespace framework {
	behaviour::behaviour(library::rcu_pointer<object> parent) noexcept
		: _parent(parent) {
	}
}