#pragma once
#include "resource.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class texture : public inherit<resource, texture, "texture"> {
		using base = inherit<resource, texture, "texture">;
	public:
		d3d11::texture_2d _texture;
		d3d11::render_target_view _rtv;
		d3d11::shader_resource_view _srv;
		d3d11::unorder_access_view _uav;
		d3d11::depth_stencil_view _dsv;
	public:
		texture(library::string_view name, library::string_view path);
		texture(nlohmann::json const& json);
		texture(d3d11::texture_2d_descript const& descript,
			d3d11::render_target_view_descript* rtv_descript,
			d3d11::shader_resource_view_descript* srv_descript,
			d3d11::unorder_access_view_descript* uav_descript,
			d3d11::depth_stencil_view_descript* dsv_descript);
		virtual ~texture(void) noexcept override = default;

		virtual void save(nlohmann::json& json) noexcept override;
	};
}