#include "texture.h"

namespace framework {
	texture::texture(wchar_t const* const path) noexcept {
		auto& device = d3d11::device::instance();
		device.create_texture_from_file(path, &_srv);
	}

}