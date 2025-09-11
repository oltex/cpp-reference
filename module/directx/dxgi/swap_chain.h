#pragma once
#include "library/system/component.h"
#include "../d3d11/texture_2d.h"
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace dxgi {
	class swap_chain : public library::component<IDXGISwapChain> {
		using base = library::component<IDXGISwapChain>;
	public:
		inline explicit swap_chain(IDXGISwapChain* component = nullptr) noexcept
			: base(component) {
		}
		inline explicit swap_chain(swap_chain const&) noexcept = default;
		inline explicit swap_chain(swap_chain&&) noexcept = default;
		inline auto operator=(swap_chain const&) noexcept -> swap_chain & = default;
		inline auto operator=(swap_chain&&) noexcept -> swap_chain & = default;
		inline ~swap_chain(void) noexcept = default;

		inline void present(void) noexcept {
			_component->Present(0, 0);
		}

		inline auto get_buffer(IID id) noexcept -> void* {
			void* component;
			_component->GetBuffer(0, id, &component);
			return component;
		}
		inline auto get_buffer_texture_2d(void) noexcept -> d3d11::texture_2d {
			ID3D11Texture2D* component = reinterpret_cast<ID3D11Texture2D*>(get_buffer(__uuidof(ID3D11Texture2D)));
			return d3d11::texture_2d(component);
		}
	};
}
