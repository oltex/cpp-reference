#pragma once
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"
#include "library/system/thread.h"

namespace winapi {
	class handle;
}

namespace framework {
	class graphic {
		d3d11::device& _device;
		d3d11::device_context _device_context;
		dxgi::swap_chain _swap_chain;

		d3d11::view_port _view_port;
		d3d11::render_target_view _render_target_view;
		d3d11::depth_stencil_view _depth_stencil_view;
	public:
		explicit graphic(::winapi::handle& handle) noexcept;
		inline explicit graphic(graphic const&) noexcept = delete;
		inline explicit graphic(graphic&&) noexcept = delete;
		inline auto operator=(graphic const&) noexcept -> graphic & = delete;
		inline auto operator=(graphic&&) noexcept -> graphic & = delete;
		~graphic(void) noexcept;

		void render_ready(void) noexcept {
			_device_context.set_view_port(_view_port);
			_device_context.set_render_target(_render_target_view, _depth_stencil_view);
			float color[4]{ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1.f };
			_device_context.clear_render_target_view(_render_target_view, color);
			_device_context.clear_depth_stencil_view(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		}
		void render_start(void) noexcept {
			_swap_chain.present();
		}
	};
}