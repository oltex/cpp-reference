#pragma once
#include "component.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class camera : public component, public componentr<camera, "camera"> {
		dmath::float4x4 _project;
	public:
		explicit camera(float field_of_view, float aspect_width, float aspect_height, float _near, float _far) noexcept;
		explicit camera(camera const&) noexcept = delete;
		explicit camera(camera&&) noexcept = delete;
		auto operator=(camera const&) noexcept -> camera & = delete;
		auto operator=(camera&&) noexcept -> camera & = delete;
		virtual ~camera(void) noexcept override = default;

		static auto buffer(void) noexcept -> d3d11::buffer&;
		auto matrix(void) noexcept -> dmath::matrix;
	};
}