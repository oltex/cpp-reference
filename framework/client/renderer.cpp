#include "renderer.h"

namespace framework {
	renderer::renderer(void) noexcept
		: component(component::type_id<renderer>()) {
	}
	void renderer::add_draw_item(library::intrusive::share_pointer<transform, 0> transform, library::share_pointer<mesh> mesh, library::share_pointer<texture> texture) {
		_draw_item.emplace_back(transform, mesh, texture);

	}
	auto renderer::get_draw_item(void) noexcept -> library::vector<draw_item>& {
		return _draw_item;
	}
}
