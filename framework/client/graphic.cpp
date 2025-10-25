#include "graphic.h"
#include "window.h"

namespace framework {
	graphic::graphic(void) noexcept
		: _device(D3D_DRIVER_TYPE_HARDWARE, static_cast<unsigned int>(D3D11_CREATE_DEVICE_DEBUG)),
		_device_context(_device.get_immediate_context()),
		_view_port(0, 0, 1424, 720, 0.f, 1.f) {

		auto dxgi_device = _device.query_interface_dxgi_device();
		auto dxgi_adapter = dxgi_device.get_parent_adapter();
		auto dxgi_factory = dxgi_adapter.get_parent_factory();
		_swap_chain = dxgi_factory.create_swap_chain(_device, window::instance(), 60, true);
		auto texture = _swap_chain.get_buffer_texture_2d();
		_render_target_view = _device.create_render_target_view(texture);
	}
	graphic::~graphic(void) noexcept {
	}

	void graphic::render(void) noexcept {
		auto rect = window::instance().get_client_rect();
		auto width = rect.right - rect.left;
		auto height = rect.bottom - rect.top;

		if (width != _view_port.Width || height != _view_port.Height) {
			_device_context.set_render_target(nullptr, nullptr);
			_render_target_view = d3d11::render_target_view(nullptr);
			_swap_chain.resize_buffer(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		}

		_device_context.set_view_port(_view_port);
		_device_context.set_render_target(&_render_target_view, nullptr);
		float color[4]{ 0.1f, 0.1f, 0.1f, 1.f };
		_device_context.clear_render_target_view(_render_target_view, color);
	}
	void graphic::present(void) noexcept {
		_swap_chain.present();
	}
	auto graphic::device(void) noexcept -> d3d11::device& {
		return _device;
	}
	auto graphic::device_context(void) noexcept -> d3d11::device_context& {
		return _device_context;
	}
}