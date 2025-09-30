#include "graphic.h"
#include "module/window/window.h"

namespace framework {
	graphic::graphic(::winapi::handle& handle) noexcept
		: _device(D3D_DRIVER_TYPE_HARDWARE, static_cast<unsigned int>(D3D11_CREATE_DEVICE_DEBUG)),
		_device_context(_device.get_immediate_context()) {

		auto dxgi_device = _device.query_interface_dxgi_device();
		auto dxgi_adapter = dxgi_device.get_parent_adapter();
		auto dxgi_factory = dxgi_adapter.get_parent_factory();
		_swap_chain = dxgi_factory.create_swap_chain(_device, handle, 60, true);
		//{
		//	auto texture = _swap_chain.get_buffer_texture_2d();
		//	_render_target_view = _device.create_render_target_view(texture);
		//}
		//{
		//	auto texture = _device.create_texture_2d(d3d11::texture_2d_descript(1424, 720, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL, 0, 0));
		//	_depth_stencil_view = _device.create_depth_stencil_view(texture);
		//}
	}
	graphic::~graphic(void) noexcept {
	}

	void graphic::render_ready(void) noexcept {
		//_device_context.set_view_port(_view_port);
		//_device_context.set_render_target(_render_target_view, _depth_stencil_view);
		//float color[4]{ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1.f };
		//_device_context.clear_render_target_view(_render_target_view, color);
		//_device_context.clear_depth_stencil_view(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
	void graphic::render_start(void) noexcept {
		_swap_chain.present();
	}
}