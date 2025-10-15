#include "camera.h"
#include "graphic.h"

namespace framework {
	camera::camera(float field_of_view, float aspect_width, float aspect_height, float _near, float _far) noexcept
		: _project(dmath::matrix::perspective_field_of_view_left_hand(dmath::convert_to_radian(field_of_view), aspect_width / aspect_height, _near, _far)) {
	}
	auto camera::buffer(void) noexcept -> d3d11::buffer& {
		static d3d11::buffer _buffer = graphic::instance()._device.create_buffer(d3d11::buffer_descript(sizeof(dmath::float4x4) * 2, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0), nullptr);
		return _buffer;
	}
	auto camera::matrix(void) noexcept -> dmath::matrix {
		return _project;
	}
}