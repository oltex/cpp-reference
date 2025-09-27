#include "camera.h"

namespace framework {
	camera::camera(float field_of_view, float aspect_width, float aspect_height, float _near, float _far) noexcept
		: component(component::type_id<camera>()) {
		_project_float4x4.store(dmath::matrix_perspective_field_of_view_left_hand(dmath::convert_to_radian(field_of_view), aspect_width / aspect_height, _near, _far));
	}
}