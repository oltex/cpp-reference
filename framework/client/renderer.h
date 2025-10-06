#pragma once
#include "component.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"

namespace framework {
	class renderer : public component, public componentr<renderer, "renderer"> {
		struct draw_item {
			library::intrusive::share_pointer<transform, 0> _transform;
			library::share_pointer<mesh> _mesh;
			library::share_pointer<texture> _texture;
		};
		library::vector<draw_item> _draw_item;
	public:
		explicit renderer(void) noexcept;
		explicit renderer(renderer const&) noexcept = delete;
		explicit renderer(renderer&&) noexcept = delete;
		auto operator=(renderer const&) noexcept -> renderer & = delete;
		auto operator=(renderer&&) noexcept -> renderer & = delete;
		virtual ~renderer(void) noexcept override = default;

		void add_draw_item(library::intrusive::share_pointer<transform, 0> transform, library::share_pointer<mesh> mesh, library::share_pointer<texture> texture);
		auto get_draw_item(void) noexcept -> library::vector<draw_item>&;
	};
}