#pragma once
#include "system.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"
#include "camera.h"
#include "transform.h"

namespace framework {
	class pipeline : public system {
		d3d11::view_port _view_port;
		d3d11::render_target_view _render_target_view;
		d3d11::depth_stencil_view _depth_stencil_view;

		library::intrusive::weak_pointer<camera, 0> _camera;
		library::intrusive::weak_pointer<transform, 0> _camera_transform;
		d3d11::buffer _camera_buffer;

		d3d11::buffer _world_matrix;
	public:
		explicit pipeline(void) noexcept;
		explicit pipeline(pipeline const&) noexcept = delete;
		explicit pipeline(pipeline&&) noexcept = delete;
		auto operator=(pipeline const&) noexcept -> pipeline & = delete;
		auto operator=(pipeline&&) noexcept -> pipeline & = delete;
		virtual ~pipeline(void) noexcept = default;

		virtual void update(void) noexcept;
		void set_camera(library::intrusive::share_pointer<camera, 0> camera, library::intrusive::share_pointer<transform, 0> transform) noexcept;
	};
}