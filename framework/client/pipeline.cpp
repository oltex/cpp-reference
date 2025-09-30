#include "pipeline.h"
#include "graphic.h"

namespace framework {
	pipeline::pipeline(void) noexcept
		: _view_port(0, 0, 1424, 720, 0.f, 1.f) {
		auto& device = graphic::instance()._device;
		auto& device_context = graphic::instance()._device_context;
		auto& swap_chain = graphic::instance()._swap_chain;
		{
			auto texture = swap_chain.get_buffer_texture_2d();
			_render_target_view = device.create_render_target_view(texture);
		}
		{
			auto texture = device.create_texture_2d(d3d11::texture_2d_descript(1424, 720, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL, 0, 0));
			_depth_stencil_view = device.create_depth_stencil_view(texture);
		}
	}
	void pipeline::update(void) noexcept {
		auto& device_context = graphic::instance()._device_context;

		device_context.set_view_port(_view_port);
		device_context.set_render_target(_render_target_view, _depth_stencil_view);
		float color[4]{ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1.f };
		device_context.clear_render_target_view(_render_target_view, color);
		device_context.clear_depth_stencil_view(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}
