#pragma once
#include "library/container/vector.h"
#include "component.h"
#include "mesh.h"
#include "transform.h"
#include "material.h"

namespace framework {
	class renderer : public componentr<renderer, "renderer"> {
		using size_type = unsigned int;
		struct draw_item {
			library::intrusive::share_pointer<transform, 0> _transform;
			library::share_pointer<mesh> _mesh;
			library::vector<library::share_pointer<material>> _material;
		};
		library::vector<draw_item> _draw_item;
	public:
		explicit renderer(void) noexcept;
		explicit renderer(renderer const&) noexcept = delete;
		explicit renderer(renderer&&) noexcept = delete;
		auto operator=(renderer const&) noexcept -> renderer & = delete;
		auto operator=(renderer&&) noexcept -> renderer & = delete;
		virtual ~renderer(void) noexcept override = default;

		auto get_draw_item(void) noexcept -> library::vector<draw_item>&;
		void set_transform(size_type index, library::intrusive::share_pointer<transform, 0>& transform) noexcept;
		void set_mesh(size_type index, library::share_pointer<mesh>& mesh) noexcept;
		void add_material(size_type index, library::share_pointer<material>& material) noexcept;
	};
}