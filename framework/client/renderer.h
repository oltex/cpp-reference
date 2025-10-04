#pragma once
#include "component.h"
#include "mesh.h"
#include "material.h"
#include "transform.h"

namespace framework {
	class renderer : public component {
		struct draw_item {
			library::share_pointer<transform> _transform;
			library::share_pointer<mesh> _mesh;
			library::share_pointer<material> _material;
		};
		library::vector<draw_item> _draw_item;
	public:
		explicit renderer(void) noexcept;
		explicit renderer(renderer const&) noexcept = delete;
		explicit renderer(renderer&&) noexcept = delete;
		auto operator=(renderer const&) noexcept -> renderer & = delete;
		auto operator=(renderer&&) noexcept -> renderer & = delete;
		virtual ~renderer(void) noexcept override = default;
	};
}