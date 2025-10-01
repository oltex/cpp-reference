#include "swap_chain.h" 
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace dxgi {
	inline swap_chain::swap_chain(IDXGISwapChain* component) noexcept
		: base(component) {
	}
	inline void swap_chain::present(void) noexcept {
		_component->Present(0, 0);
	}
	inline auto swap_chain::get_buffer(IID id) noexcept -> void* {
		void* component;
		auto result = _component->GetBuffer(0, id, &component);
		return component;
	}
	inline auto swap_chain::get_buffer_texture_2d(void) noexcept -> d3d11::texture_2d {
		ID3D11Texture2D* component = reinterpret_cast<ID3D11Texture2D*>(get_buffer(__uuidof(ID3D11Texture2D)));
		return d3d11::texture_2d(component);
	}
}