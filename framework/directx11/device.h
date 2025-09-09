#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include "library/system/component.h"
#include "library/pattern/singleton.h"

namespace directx {
	class device : public library::component<ID3D11Device>, public library::singleton<device> {
		friend class library::singleton<device>;
		using base = library::component<ID3D11Device>;

		inline explicit device(void) noexcept
			: base() {
			D3D_FEATURE_LEVEL freature_level{};
			::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &_component, &freature_level, nullptr);
		}
		inline explicit device(device const&) noexcept = delete;
		inline explicit device(device&&) noexcept = delete;
		inline auto operator=(device const&) noexcept -> device & = delete;
		inline auto operator=(device&&) noexcept -> device & = delete;
		inline ~device(void) noexcept = default;

		inline auto immediate_context(void) const noexcept -> ID3D11DeviceContext* {
			ID3D11DeviceContext* context;
			_component->GetImmediateContext(&context);
			return context;
		}
		inline auto deferred_context(void) const noexcept -> ID3D11DeviceContext* {
			ID3D11DeviceContext* context;
			_component->CreateDeferredContext(0, &context);
			return context;
		}

		inline auto texture_2d(D3D11_TEXTURE2D_DESC& desc, D3D11_SUBRESOURCE_DATA* data = nullptr) const noexcept -> ID3D11Texture2D* {
			ID3D11Texture2D* texture;
			_component->CreateTexture2D(&desc, data, &texture);
			return texture;
		}
	};
}