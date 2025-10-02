#include "graphic.h"
#include "window.h"

namespace framework {
	graphic::graphic(void) noexcept
		: _device(D3D_DRIVER_TYPE_HARDWARE, static_cast<unsigned int>(D3D11_CREATE_DEVICE_DEBUG)),
		_device_context(_device.get_immediate_context()) {

		auto dxgi_device = _device.query_interface_dxgi_device();
		auto dxgi_adapter = dxgi_device.get_parent_adapter();
		auto dxgi_factory = dxgi_adapter.get_parent_factory();
		_swap_chain = dxgi_factory.create_swap_chain(_device, window::instance(), 60, true);
	}
	graphic::~graphic(void) noexcept {
	}

	void graphic::render_start(void) noexcept {
		_swap_chain.present();
	}
}