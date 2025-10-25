#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "../d3d11/texture_2d.h"
#include <dxgi.h>

namespace dxgi {
	struct swap_chain_descript : public DXGI_SWAP_CHAIN_DESC {
		inline explicit swap_chain_descript(unsigned int width, unsigned int height, unsigned int refresh_rate_numerator, unsigned int refresh_rate_denominator,
			DXGI_FORMAT format, DXGI_MODE_SCANLINE_ORDER ordering, DXGI_MODE_SCALING scaling, UINT count, UINT quality, DXGI_USAGE buffer_usage, UINT buffer_count, HWND output_window, BOOL windowed, DXGI_SWAP_EFFECT swap_effect, UINT flags) noexcept {
			BufferDesc.Width = width;
			BufferDesc.Height = height;
			BufferDesc.RefreshRate.Numerator = refresh_rate_numerator;
			BufferDesc.RefreshRate.Denominator = refresh_rate_denominator;
			BufferDesc.Format = format;
			BufferDesc.ScanlineOrdering = ordering;
			BufferDesc.Scaling = scaling;
			SampleDesc.Count = count;
			SampleDesc.Quality = quality;
			BufferUsage = buffer_usage;
			BufferCount = buffer_count;
			OutputWindow = output_window;
			Windowed = windowed;
			SwapEffect = swap_effect;
			Flags = flags;
		}
		inline explicit swap_chain_descript(swap_chain_descript const&) noexcept = default;
		inline explicit swap_chain_descript(swap_chain_descript&&) noexcept = default;
		inline auto operator=(swap_chain_descript const&) noexcept -> swap_chain_descript & = default;
		inline auto operator=(swap_chain_descript&&) noexcept -> swap_chain_descript & = default;
		inline ~swap_chain_descript(void) noexcept = default;
	};

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
		inline void resize_buffer(unsigned int count, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int flag) noexcept;
	};
}
