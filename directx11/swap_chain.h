#pragma once
#pragma comment(lib,"d3d11.lib")
#include <d3d11.h>

namespace directx11 {
	class swap_chain final {
	public:
		inline explicit swap_chain(void) noexcept {
		}
		inline ~swap_chain(void) noexcept {
			swapchain->Release();
		}
	public:

	private:
		IDXGISwapChain* swapchain;
	};
}