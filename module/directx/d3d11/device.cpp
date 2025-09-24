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
		_component->CreateDeferredContext(0, &context);
		return device_context(context);
	}
	inline auto device::query_interface_dxgi_device(void) noexcept -> dxgi::device {
		IDXGIDevice* device = reinterpret_cast<IDXGIDevice*>(base::query_interface(__uuidof(IDXGIDevice)));
		return dxgi::device(device);
	}
	inline auto device::create_texture_2d(texture_2d_descript descript, D3D11_SUBRESOURCE_DATA* data) noexcept -> texture_2d {
		ID3D11Texture2D* texture;
		_component->CreateTexture2D(&descript, data, &texture);
		return texture_2d(texture);
	}
	inline void device::create_texture_from_file(wchar_t const* const path, shader_resource_view* srv) noexcept {
		if (nullptr != library::string_string(path, L".dds"))
			DirectX::CreateDDSTextureFromFile(_component, path, nullptr, nullptr == srv ? nullptr : &srv->data());
		else
			DirectX::CreateWICTextureFromFile(_component, path, nullptr, nullptr == srv ? nullptr : &srv->data());
	}

	inline auto device::create_render_target_view(texture_2d& texture, D3D11_RENDER_TARGET_VIEW_DESC* desc) const noexcept -> render_target_view {
		ID3D11RenderTargetView* view;
		_component->CreateRenderTargetView(texture.data(), desc, &view);
		return render_target_view(view);
	}
	inline auto device::create_shader_resource_view(texture_2d& texture, D3D11_SHADER_RESOURCE_VIEW_DESC* desc) const noexcept -> shader_resource_view {
		ID3D11ShaderResourceView* view;
		_component->CreateShaderResourceView(texture.data(), desc, &view);
		return shader_resource_view(view);
	}
	inline auto device::create_unorder_access_view(texture_2d& texture, D3D11_UNORDERED_ACCESS_VIEW_DESC* desc) const noexcept -> unorder_access_view {
		ID3D11UnorderedAccessView* view;
		_component->CreateUnorderedAccessView(texture.data(), desc, &view);
		return unorder_access_view(view);
	}
	inline auto device::create_depth_stencil_view(texture_2d& texture, D3D11_DEPTH_STENCIL_VIEW_DESC* desc) const noexcept -> depth_stencil_view {
		ID3D11DepthStencilView* view;
		_component->CreateDepthStencilView(texture.data(), desc, &view);
		return depth_stencil_view(view);
	}
}