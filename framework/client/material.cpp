#include "material.h"
#include "resource.h"
#include "library/tinygltf/tiny_gltf.h"

namespace framework {
	material::material(wchar_t const* const path) noexcept {
	}
	void material::set_shader(library::share_pointer<shader>& shader) noexcept {
		_shader = shader;
	}
	void material::add_texture(library::share_pointer<texture>& texture) noexcept {
		_texture.emplace_back(texture);
	}
	void material::set_pixel_shader_resource(d3d11::device_context& device_context) noexcept {
		for (auto index = 0u; index < _texture.size(); ++index) {
			device_context.set_pixel_shader_resource(index, 1, &_texture[index]->_srv.data());
		}
	}
}
