#pragma once
#include "library/system/component.h"
#include "library/pattern/singleton.h"
#include "../dxgi/device.h"
#include "texture_2d.h"
#include "device_context.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <cassert>

namespace d3d11 {
	class declspec_dll device : public library::component<ID3D11Device>, public library::singleton<device, true, true> {
		friend class library::singleton<device, true, true>;
		using base = library::component<ID3D11Device>;

		inline explicit device(D3D_DRIVER_TYPE driver_type, unsigned int flag) noexcept
			: base() {
			auto result = ::D3D11CreateDevice(nullptr, driver_type, 0, flag, nullptr, 0, D3D11_SDK_VERSION, &_component, nullptr, nullptr);
			assert(SUCCEEDED(result));
		}
		inline explicit device(device const&) noexcept = delete;
		inline explicit device(device&&) noexcept = delete;
		inline auto operator=(device const&) noexcept -> device & = delete;
		inline auto operator=(device&&) noexcept -> device & = delete;
		inline ~device(void) noexcept = default;
	public:
		inline auto get_immediate_context(void) const noexcept -> device_context {
			ID3D11DeviceContext* context;
			_component->GetImmediateContext(&context);
			return device_context(context);
		}
		inline auto create_deferred_context(void) const noexcept -> device_context {
			ID3D11DeviceContext* context;
			_component->CreateDeferredContext(0, &context);
			return device_context(context);
		}
		inline auto query_interface_dxgi_device(void) noexcept -> dxgi::device {
			IDXGIDevice* device = reinterpret_cast<IDXGIDevice*>(base::query_interface(__uuidof(IDXGIDevice)));
			return dxgi::device(device);
		}
	};
}