#include "renderer.h"

namespace framework {
	renderer::renderer(void) noexcept
		: component(component::type_id<renderer>()) {
	}
	auto renderer::get_draw_item(void) noexcept -> library::vector<draw_item>& {
		return _draw_item;
	}
	void renderer::set_transform(size_type index, library::intrusive::share_pointer<transform, 0>& transform) noexcept {
		if (_draw_item.size() < index + 1) 
			_draw_item.resize(index + 1);
		_draw_item[index]._transform = transform;
	}
	void renderer::set_mesh(size_type index, library::share_pointer<mesh>& mesh) noexcept {
		if (_draw_item.size() < index + 1)
			_draw_item.resize(index + 1);
		_draw_item[index]._mesh = mesh;
	}
	void renderer::add_material(size_type index, library::share_pointer<material>& material) noexcept {
		if (_draw_item.size() < index + 1)
			_draw_item.resize(index + 1);
		_draw_item[index]._material.emplace_back(material);
	}
}
