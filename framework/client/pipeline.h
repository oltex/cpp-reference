#pragma once
#include "library/container/list.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"
#include "system.h"
#include "camera.h"
#include "transform.h"
#include "renderer.h"

namespace framework {
	class pipeline : public system {
		d3d11::view_port _view_port;
		d3d11::render_target_view _render_target_view;
		d3d11::depth_stencil_view _depth_stencil_view;

		//library::intrusive::weak_pointer<camera, 0> _camera;
		//library::intrusive::weak_pointer<transform, 0> _camera_transform;
		//library::list<library::intrusive::weak_pointer<renderer, 0>> _renderer;

	public:
		explicit pipeline(void) noexcept;
		explicit pipeline(pipeline const&) noexcept = delete;
		explicit pipeline(pipeline&&) noexcept = delete;
		auto operator=(pipeline const&) noexcept -> pipeline & = delete;
		auto operator=(pipeline&&) noexcept -> pipeline & = delete;
		virtual ~pipeline(void) noexcept = default;

		virtual void update(void) noexcept;
		//virtual void add_component(library::string const& key, library::vector<library::intrusive::share_pointer<component, 0>>  const& component) noexcept override;
	};
}