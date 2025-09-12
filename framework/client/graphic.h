#pragma once
#include "client.h"
#include "module/window/handle.h"

#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class graphic {
		d3d11::device& _device;
		dxgi::swap_chain _swap_chain;

		d3d11::render_target_view _render_target_view;
		d3d11::depth_stencil_view _depth_stencil_view;
	public:
		inline explicit graphic(::window::handle& handle) noexcept
			: _device(d3d11::device::construct(D3D_DRIVER_TYPE_HARDWARE, static_cast<unsigned int>(D3D11_CREATE_DEVICE_DEBUG))) {
			auto dxgi_device = _device.query_interface_dxgi_device();
			auto dxgi_adapter = dxgi_device.get_parent_adapter();
			auto dxgi_factory = dxgi_adapter.get_parent_factory();
			_swap_chain = dxgi_factory.create_swap_chain(handle, 60, true);
			{
				auto texture = _swap_chain.get_buffer_texture_2d();
				_render_target_view = texture.create_render_target_view();
			}
			{
				d3d11::texture_2d texture(d3d11::texture_2d_descript(1920, 1080, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL, 0, 0));
				_depth_stencil_view = texture.create_depth_stencil_view();
			}
		}
		inline ~graphic(void) noexcept {
			d3d11::device::destruct();
		}
	};
}