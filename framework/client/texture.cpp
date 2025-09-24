#include "texture.h"
#include "graphic.h"

namespace framework {
	texture::texture(wchar_t const* const path) noexcept {
		graphic::instance()._device.create_texture_from_file(path, &_srv);
	}

	texture::texture(texture const&) noexcept
	{
	}

}