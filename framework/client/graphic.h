#pragma once
#include "library/pattern/singleton.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"
#include "library/system/thread.h"

namespace winapi {
	class handle;
}

namespace framework {
	class graphic : public library::singleton<graphic, true> {
		friend class library::singleton<graphic, true>;
		friend class texture;
		friend class mesh;

		d3d11::device _device;
		d3d11::device_context _device_context;
		dxgi::swap_chain _swap_chain;
		d3d11::view_port _view_port;
		d3d11::render_target_view _render_target_view;
		d3d11::depth_stencil_view _depth_stencil_view;
	public:
		explicit graphic(::winapi::handle& handle) noexcept;
		explicit graphic(graphic const&) noexcept = delete;
		explicit graphic(graphic&&) noexcept = delete;
		auto operator=(graphic const&) noexcept -> graphic & = delete;
		auto operator=(graphic&&) noexcept -> graphic & = delete;
		~graphic(void) noexcept;

		void render_ready(void) noexcept;
		void render_start(void) noexcept;
	};
}