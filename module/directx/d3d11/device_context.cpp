#include "device_context.h"

namespace d3d11 {
	inline device_context::device_context(ID3D11DeviceContext* component) noexcept
		: base(component) {
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
	inline void device_context::set_vertex_buffer(unsigned int start_slot, unsigned int number_buffer, ID3D11Buffer** vertex_buffer, unsigned int* stride, unsigned int* offset) noexcept {
		_component->IASetVertexBuffers(start_slot, number_buffer, vertex_buffer, stride, offset);
	}
	inline void device_context::set_index_buffer(ID3D11Buffer* index_buffer, DXGI_FORMAT format, unsigned int offset) noexcept {
		_component->IASetIndexBuffer(index_buffer, format, offset);
	}
	inline void d3d11::device_context::set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept {
		_component->IASetPrimitiveTopology(topology);
	}
	inline void device_context::draw_index(unsigned int index_count, unsigned int start_index, unsigned int base_vertex) noexcept {
		_component->DrawIndexed(index_count, start_index, base_vertex);
		//_component->DrawIndexedInstanced()
	}
};
