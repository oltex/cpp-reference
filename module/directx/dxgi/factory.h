#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "swap_chain.h"
#include <dxgi.h>

namespace winapi {
	class handle;
}
namespace d3d11 {
	class device;
}

namespace dxgi {
	class declspec_dll factory : library::component<IDXGIFactory> {
		using base = library::component<IDXGIFactory>;
	public:
		inline explicit factory(IDXGIFactory* component) noexcept
			: base(component) {
		};
		inline explicit factory(factory const&) noexcept = default;
		inline explicit factory(factory&&) noexcept = default;
		inline auto operator=(factory const&) noexcept -> factory & = default;
		inline auto operator=(factory&&) noexcept -> factory & = default;
		inline ~factory(void) noexcept = default;

		inline auto create_swap_chain(d3d11::device& device, winapi::handle& handle, unsigned int numerator, bool window) noexcept -> dxgi::swap_chain;
	};
}

//
//DXGI_SWAP_CHAIN_DESC desc{
//	.BufferDesc{
//		.Width = 1920,
//		.Height = 1080,
//		.RefreshRate{
//			.Numerator = 60,
//			.Denominator = 1
//		},
//		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
//		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
//		.Scaling = DXGI_MODE_SCALING_UNSPECIFIED
//	},
//	.SampleDesc{
//		.Count = 1,
//		.Quality = 0
//	},
//	.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
//	.BufferCount = 1,
//	.OutputWindow = nullptr,
//	.Windowed = true,
//	.SwapEffect = DXGI_SWAP_EFFECT_DISCARD
//};
//factory->CreateSwapChain(instance.data(), &desc, &_component);
