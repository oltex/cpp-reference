#pragma once
#include "component.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class camera : public component {
		dmath::float4x4 _project_float4x4;
	public:
		inline explicit camera(float field_of_view, float aspect_width, float aspect_height, float _near, float _far) noexcept
			: component(component::type_id<camera>()) {
			_project_float4x4.store(dmath::matrix_perspective_field_of_view_left_hand(dmath::convert_to_radian(field_of_view), aspect_width / aspect_height, _near, _far));
		}
		inline explicit camera(camera const&) noexcept = delete;
		inline explicit camera(camera&&) noexcept = delete;
		inline auto operator=(camera const&) noexcept -> camera & = delete;
		inline auto operator=(camera&&) noexcept -> camera & = delete;
		inline virtual ~camera(void) noexcept override = default;
	};
}