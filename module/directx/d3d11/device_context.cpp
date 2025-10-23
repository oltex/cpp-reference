#include "device_context.h"
#include "texture_2d.h"
#include <cassert>
#include "../DirectXTK/ScreenGrab.h"
#ifdef _DEBUG
#pragma comment(lib, "DirectXTK/DirectXTKd.lib")
#else
#pragma comment(lib, "DirectXTK/DirectXTK.lib")
#endif

namespace d3d11 {
	inline device_context::device_context(ID3D11DeviceContext* component) noexcept
		: base(component) {
	}
	inline void device_context::save_texture_from_file(texture_2d const& texture, wchar_t const* const path) noexcept {
		auto result = DirectX::SaveDDSTextureToFile(_component, texture, path);
		assert(SUCCEEDED(result));
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
	inline void device_context::set_input_layout(input_layout& input_layout) noexcept {
		_component->IASetInputLayout(input_layout.data());
	}
	inline void device_context::set_vertex_shader(vertex_shader& vertex_shader) noexcept {
		_component->VSSetShader(vertex_shader.data(), nullptr, 0);
	}
	inline void device_context::set_vertex_shader_constant_buffer(unsigned int start_slot, unsigned int number_buffer, ID3D11Buffer* constant_buffer[]) noexcept {
		_component->VSSetConstantBuffers(start_slot, number_buffer, constant_buffer);
	}
	inline void device_context::set_pixel_shader(pixel_shader& pixel_shader) noexcept {
		_component->PSSetShader(pixel_shader.data(), nullptr, 0);
	}
	inline void device_context::set_pixel_shader_resource(unsigned int start_slot, unsigned int number_view, ID3D11ShaderResourceView* shader_resource_view[]) noexcept {
		_component->PSSetShaderResources(start_slot, number_view, shader_resource_view);
	}
	inline void device_context::set_pixel_shader_sampler(unsigned int start_slot, unsigned int number_sampler, ID3D11SamplerState* sampler_state[]) noexcept {
		_component->PSSetSamplers(start_slot, number_sampler, sampler_state);
	}


	inline void device_context::set_vertex_buffer(unsigned int start_slot, unsigned int number_buffer, ID3D11Buffer* vertex_buffer[], unsigned int stride[], unsigned int offset[]) noexcept {
		_component->IASetVertexBuffers(start_slot, number_buffer, vertex_buffer, stride, offset);
	}
	inline void device_context::set_index_buffer(buffer& index_buffer, DXGI_FORMAT format, unsigned int offset) noexcept {
		_component->IASetIndexBuffer(index_buffer.data(), format, offset);
	}
	inline auto device_context::map(ID3D11Resource* resource, unsigned int sub_resource, D3D11_MAP type, unsigned int flag) noexcept -> map_sub_resource {
		map_sub_resource map_resource;
		auto result = _component->Map(resource, sub_resource, type, flag, &map_resource);
		assert(SUCCEEDED(result));
		return map_resource;
	}
	inline void device_context::unmap(ID3D11Resource* resource, unsigned int sub_resource) noexcept {
		_component->Unmap(resource, sub_resource);
	}
	inline void d3d11::device_context::set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept {
		_component->IASetPrimitiveTopology(topology);
	}
	inline void device_context::draw_index(unsigned int index_count, unsigned int start_index, unsigned int base_vertex) noexcept {
		_component->DrawIndexed(index_count, start_index, base_vertex);
		//_component->DrawIndexedInstanced()
	}
};
