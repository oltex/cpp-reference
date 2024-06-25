#include "object.h"

namespace window {
	object::~object(void) noexcept {
		DeleteObject(_hgdiobj);
	}
}