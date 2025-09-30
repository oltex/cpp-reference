#include "device.h"
#include "library/function.h"
#include <cassert>
#include "../DirectXTK/DDSTextureLoader.h"
#include "../DirectXTK/WICTextureLoader.h"
#ifdef _DEBUG
#pragma comment(lib, "DirectXTK/DirectXTKd.lib")
#else
#pragma comment(lib, "DirectXTK/DirectXTK.lib")
#endif

//namespace library {
//	template<>
//	d3d11::device* singleton<d3d11::device, true, true>::_instance = nullptr;
//	template<>
//	template<>
//	declspec_dll auto singleton<d3d11::device, true, true>::construct<D3D_DRIVER_TYPE, unsigned int>(D3D_DRIVER_TYPE&& driver_type, unsigned int&& flag) noexcept -> d3d11::device& {
//		_instance = library::allocate<d3d11::device>();
//		::new(reinterpret_cast<void*>(_instance)) d3d11::device(driver_type, flag);
//		return *_instance;
//	}
//	template<>
//	declspec_dll auto singleton<d3d11::device, true, true>::instance(void) noexcept -> d3d11::device& {
//		return *_instance;
//	}
//	template<>
//	declspec_dll void singleton<d3d11::device, true, true>::destruct(void) noexcept {
//		_instance->~device();
//		library::deallocate(_instance);
//	}
//	template class declspec_dll singleton<d3d11::device, true, true>;
//}

namespace d3d11 {
	inline device::device(D3D_DRIVER_TYPE driver_type, unsigned int flag) noexcept
		: base() {
		auto result = ::D3D11CreateDevice(nullptr, driver_type, 0, flag, nullptr, 0, D3D11_SDK_VERSION, &_component, nullptr, nullptr);
		assert(SUCCEEDED(result));
	}
	inline auto device::get_immediate_context(void) const noexcept -> device_context {
		ID3D11DeviceContext* context;
		_component->GetImmediateContext(&context);
		return device_context(context);
	}
	inline auto device::create_deferred_context(void) const noexcept -> device_context {
		ID3D11DeviceContext* context;
		auto result = _component->CreateDeferredContext(0, &context);
		assert(SUCCEEDED(result));
		return device_context(context);
	}
	inline auto device::query_interface_dxgi_device(void) noexcept -> dxgi::device {
		IDXGIDevice* device = reinterpret_cast<IDXGIDevice*>(base::query_interface(__uuidof(IDXGIDevice)));
		return dxgi::device(device);
	}
	inline auto device::create_input_layout(library::vector<input_element_descript>& descript, d3d::blob& blob) const noexcept -> input_layout {
		ID3D11InputLayout* component;
		auto result = _component->CreateInputLayout(descript.data(), descript.size(), blob.get_buffer_pointer(), blob.get_buffer_size(), &component);
		assert(SUCCEEDED(result));
		return input_layout(component);
	}
	inline auto device::create_vertex_shader(d3d::blob& blob) noexcept -> vertex_shader {
		ID3D11VertexShader* component;
		auto result = _component->CreateVertexShader(blob.get_buffer_pointer(), blob.get_buffer_size(), nullptr, &component);
		assert(SUCCEEDED(result));
		return vertex_shader(component);
	}
	inline auto device::create_pixel_shader(d3d::blob& blob) noexcept -> pixel_shader {
		ID3D11PixelShader* component;
		auto result = _component->CreatePixelShader(blob.get_buffer_pointer(), blob.get_buffer_size(), nullptr, &component);
		assert(SUCCEEDED(result));
		return pixel_shader(component);
	}
	inline auto device::create_rasterize_state(rasterize_descript const& descript) const noexcept -> rasterize_state {
		ID3D11RasterizerState* component;
		auto result = _component->CreateRasterizerState(&descript, &component);
		assert(SUCCEEDED(result));
		return rasterize_state(component);
	}
	inline auto device::create_blend_state(blend_descript const& descript) const noexcept -> blend_state {
		ID3D11BlendState* component;
		_component->CreateBlendState(&descript, &component);
		return blend_state(component);
	}
	inline auto device::create_depth_stencil_state(depth_stencil_descript const& descript) const noexcept -> depth_stencil_state {
		ID3D11DepthStencilState* component;
		_component->CreateDepthStencilState(&descript, &component);
		return depth_stencil_state(component);
	}
	inline auto device::create_sampler_state(sampler_descript const& descript) const noexcept -> depth_stencil_state {
		ID3D11SamplerState* component;
		auto result = _component->CreateSamplerState(&descript, &component);
		assert(SUCCEEDED(result));
		return depth_stencil_state();
	}
	inline auto device::create_buffer(buffer_descript const& descript, sub_resource_data const& data) const noexcept -> buffer {
		ID3D11Buffer* component;
		_component->CreateBuffer(&descript, &data, &component);
		return buffer(component);
	}
	inline auto device::create_texture_2d(texture_2d_descript const& descript, sub_resource_data* data) noexcept -> texture_2d {
		ID3D11Texture2D* component;
		_component->CreateTexture2D(&descript, data, &component);
		return texture_2d(component);
	}
	inline auto device::create_texture_from_file(wchar_t const* const path) noexcept -> library::pair<texture_2d, shader_resource_view> {
		ID3D11Resource* resource_component;
		ID3D11ShaderResourceView* shader_component;
		if (nullptr != library::string_string(path, L".dds"))
			DirectX::CreateDDSTextureFromFile(_component, path, &resource_component, &shader_component);
		else
			DirectX::CreateWICTextureFromFile(_component, path, &resource_component, &shader_component);
		ID3D11Texture2D* texture_component;
		resource_component->QueryInterface<ID3D11Texture2D>(&texture_component);
		resource_component->Release();
		return library::pair<texture_2d, shader_resource_view>(texture_component, shader_component);
	}
	inline auto device::create_render_target_view(texture_2d& texture, render_target_view_descript* desc) const noexcept -> render_target_view {
		ID3D11RenderTargetView* view;
		_component->CreateRenderTargetView(texture.data(), desc, &view);
		return render_target_view(view);
	}
	inline auto device::create_shader_resource_view(texture_2d& texture, shader_resource_view_descript* desc) const noexcept -> shader_resource_view {
		ID3D11ShaderResourceView* view;
		_component->CreateShaderResourceView(texture.data(), desc, &view);
		return shader_resource_view(view);
	}
	inline auto device::create_unorder_access_view(texture_2d& texture, unorder_access_view_descript* desc) const noexcept -> unorder_access_view {
		ID3D11UnorderedAccessView* view;
		_component->CreateUnorderedAccessView(texture.data(), desc, &view);
		return unorder_access_view(view);
	}
	inline auto device::create_depth_stencil_view(texture_2d& texture, depth_stencil_view_descript* desc) const noexcept -> depth_stencil_view {
		ID3D11DepthStencilView* view;
		_component->CreateDepthStencilView(texture.data(), desc, &view);
		return depth_stencil_view(view);
	}
}