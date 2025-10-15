#include "collider.h"

namespace framework {
	collider::collider(void) noexcept
		: _bound(DirectX::BoundingBox({ 0,0,0 }, { 1,1,1 })) {
	}
}
