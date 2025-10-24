#include "texture.h"
#include "graphic.h"
#include "library/container/tuple.h"

namespace framework {
	texture::texture(library::string_view name, library::string_view path)
		: base(name) {
		auto& graphic = graphic::instance();
		auto& device = graphic._device;
		auto& device_context = graphic._device_context;
		library::tie(_texture, _srv) = device.create_texture_from_file(library::wstring(path).data());

		library::wstring save_path = "resource/" + _name + ".dds";
		device_context.save_texture_from_file(_texture, save_path.data());
	}
	texture::texture(const nlohmann::json& json) 
		: base(json) {
		library::wstring path = "resource/" + _name + ".dds";
		library::tie(_texture, _srv) = graphic::instance()._device.create_texture_from_file(library::wstring(path).data());
	}
	texture::texture(d3d11::texture_2d_descript const& descript,
		d3d11::render_target_view_descript* rtv_descript,
		d3d11::shader_resource_view_descript* srv_descript,
		d3d11::unorder_access_view_descript* uav_descript,
		d3d11::depth_stencil_view_descript* dsv_descript) {

		auto& device = graphic::instance()._device;
		_texture = device.create_texture_2d(descript, nullptr);

		if (D3D11_BIND_RENDER_TARGET & descript.BindFlags)
			_rtv = device.create_render_target_view(_texture, rtv_descript);
		if (D3D11_BIND_SHADER_RESOURCE & descript.BindFlags)
			_srv = device.create_shader_resource_view(_texture, srv_descript);
		if (D3D11_BIND_UNORDERED_ACCESS & descript.BindFlags)
			_uav = device.create_unorder_access_view(_texture, uav_descript);
		if (D3D11_BIND_DEPTH_STENCIL & descript.BindFlags)
			_dsv = device.create_depth_stencil_view(_texture, dsv_descript);
	}
	void texture::save(nlohmann::json& json) noexcept {
		resource::save(json);
	}
}