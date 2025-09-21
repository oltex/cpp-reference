#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "../d3d11/texture_2d.h"
#include <dxgi.h>

namespace dxgi {
	class declspec_dll swap_chain : library::component<IDXGISwapChain> {
		using base = library::component<IDXGISwapChain>;
	public:
		inline explicit swap_chain(IDXGISwapChain* component = nullptr) noexcept;
		inline explicit swap_chain(swap_chain const&) noexcept = default;
		inline explicit swap_chain(swap_chain&&) noexcept = default;
		inline auto operator=(swap_chain const&) noexcept -> swap_chain & = default;
		inline auto operator=(swap_chain&&) noexcept -> swap_chain & = default;
		inline ~swap_chain(void) noexcept = default;

		inline void present(void) noexcept;
		inline auto get_buffer(IID id) noexcept -> void*;
		inline auto get_buffer_texture_2d(void) noexcept -> d3d11::texture_2d;
	};
}
