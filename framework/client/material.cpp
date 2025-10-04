#include "material.h"
#include "resource.h"

namespace framework {
	void material::add_texture(char const* const name) noexcept {
		auto texture = resources::instance().find_resource<framework::texture>(name);
		_texture.emplace_back(texture);
	}
}
