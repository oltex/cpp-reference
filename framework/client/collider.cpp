#include "collider.h"

namespace framework {
	collider::collider(void) noexcept
		: component(component::type_id<collider>()), _bound(DirectX::BoundingBox({ 0,0,0 }, { 1,1,1 })) {
	}
}
