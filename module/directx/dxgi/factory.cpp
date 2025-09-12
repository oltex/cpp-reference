#include "factory.h"
#include "../d3d11/device.h"
#include "module/window/handle.h"
#include <cassert>

namespace dxgi {
	inline auto factory::create_swap_chain(window::handle& handle, unsigned int Numerator, bool window) noexcept -> dxgi::swap_chain {
		auto rect = handle.get_client_rect();
		DXGI_SWAP_CHAIN_DESC desc{
			.BufferDesc{
				.Width = static_cast<unsigned int>(rect.right - rect.left),
				.Height = static_cast<unsigned int>(rect.bottom - rect.top),
				.RefreshRate{
					.Numerator = Numerator,
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
			.BufferCount = 2,
			.OutputWindow = handle.data(),
			.Windowed = window,
			.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD //DXGI_SWAP_EFFECT_DISCARD 
		};
		IDXGISwapChain* component;
		auto result = _component->CreateSwapChain(d3d11::device::instance().data(), &desc, &component);
		assert(SUCCEEDED(result));
		return dxgi::swap_chain(component);
	}
}