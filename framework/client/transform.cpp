#include "transform.h"

namespace framework {
	transform::transform(void) noexcept
		: component(component::type_id<transform>())  {
		_float4x4.store(dmath::matrix_identity());
	};
}