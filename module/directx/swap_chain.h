#pragma once
#pragma comment(lib, "d3d11.lib")
#include "device.h"
#include "library/system/component.h"
#include <d3d11.h>

namespace directx {
	class swap_chain : public library::component<IDXGISwapChain> {
		using base = library::component<IDXGISwapChain>;
	public:
		inline explicit swap_chain(void) noexcept
			: base() {
			auto& instance = directx::device::instance();
			IDXGIDevice* device;
			instance.data()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));
			IDXGIAdapter* adapter = nullptr;
			device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));
			IDXGIFactory* factory = nullptr;
			adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
			DXGI_SWAP_CHAIN_DESC desc{
				.BufferDesc{
					.Width = 1920,
					.Height = 1080,
					.RefreshRate{
						.Numerator = 60,
						.Denominator = 1
					},
					.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
					.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
					.Scaling = DXGI_MODE_SCALING_UNSPECIFIED
				},
				.SampleDesc{
					.Count = 1,
					.Quality = 0
				},
				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
				.BufferCount = 1,
				.OutputWindow = nullptr,
				.Windowed = true,
				.SwapEffect = DXGI_SWAP_EFFECT_DISCARD
			};
			factory->CreateSwapChain(instance.data(), &desc, &_component);

			factory->Release();
			adapter->Release();
			device->Release();
		}
		inline explicit swap_chain(swap_chain const&) noexcept = delete;
		inline explicit swap_chain(swap_chain&&) noexcept = default;
		inline auto operator=(swap_chain const&) noexcept -> swap_chain & = delete;
		inline auto operator=(swap_chain&&) noexcept -> swap_chain & = default;
		inline ~swap_chain(void) noexcept = default;

		inline void present(void) noexcept {
			_component->Present(0, 0);
		}
		inline auto texture_2d(void) noexcept -> ID3D11Texture2D* {
			ID3D11Texture2D* texture;
			_component->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));
			return texture;
		}
	};
}