#include "device_context.h"

namespace d3d11 {
	inline device_context::device_context(ID3D11DeviceContext* device_context) noexcept
		: base(device_context) {
	}
	inline void device_context::set_view_port(unsigned int number, view_port* view_port) noexcept {
		_component->RSSetViewports(number, view_port);
	}
	inline void device_context::set_view_port(view_port& view_port) noexcept {
		_component->RSSetViewports(1, &view_port);
	}
	//inline auto device_context::get_view_port(void) noexcept -> view_port {
	//	unsigned int count;
	//	_component->RSGetViewports(&count, nullptr);
	//}
	inline void device_context::set_render_target(render_target_view& rtv, depth_stencil_view& dsv) noexcept {
		_component->OMSetRenderTargets(1, &rtv.data(), dsv.data());
	}
	inline void device_context::clear_render_target_view(render_target_view& rtv, float color[4]) noexcept {
		_component->ClearRenderTargetView(rtv.data(), color);
	}
	inline void device_context::clear_depth_stencil_view(depth_stencil_view& dsv, unsigned int clear_flag, float depth, unsigned char stencil) noexcept {
		_component->ClearDepthStencilView(dsv.data(), clear_flag, depth, stencil);
	}
	//m_pContext->RSSetViewports(1, &Viewport);
};
