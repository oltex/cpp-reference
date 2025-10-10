#include "camera_move.h"

namespace framework {
	camera_move::camera_move(library::intrusive::share_pointer<object, 0>& parent) noexcept
		: behaviour(component::type_id<camera_move>(), parent) {
	}

	void camera_move::update(void) noexcept {
		_parent.lock();
	}
}
