#pragma once
#include "resource.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class texture : public resource {
		d3d11::texture_2d _texture;
		d3d11::render_target_view _rtv;
		d3d11::shader_resource_view _srv;
		d3d11::unorder_access_view _uav;
		d3d11::depth_stencil_view _dsv;
	public:
		explicit texture(wchar_t const* const path) noexcept;
		explicit texture(d3d11::texture_2d_descript const& descript,
			d3d11::render_target_view_descript* rtv_descript,
			d3d11::shader_resource_view_descript* srv_descript,
			d3d11::unorder_access_view_descript* uav_descript,
			d3d11::depth_stencil_view_descript* dsv_descript) noexcept;
		inline explicit texture(texture const&) noexcept = default;
		inline explicit texture(texture&&) noexcept = delete;
		inline auto operator=(texture const&) noexcept -> texture & = delete;
		inline auto operator=(texture&&) noexcept -> texture & = delete;
		inline virtual ~texture(void) noexcept override = default;
	};
}