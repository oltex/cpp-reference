#pragma once
#include "component.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class camera : public component {
	public:
		dmath::float4x4 _project_float4x4;
	public:
		explicit camera(float field_of_view, float aspect_width, float aspect_height, float _near, float _far) noexcept;
		explicit camera(camera const&) noexcept = delete;
		explicit camera(camera&&) noexcept = delete;
		auto operator=(camera const&) noexcept -> camera & = delete;
		auto operator=(camera&&) noexcept -> camera & = delete;
		virtual ~camera(void) noexcept override = default;
	};
}