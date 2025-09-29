#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct rasterize_descript : public D3D11_RASTERIZER_DESC {
		inline explicit rasterize_descript(void) noexcept = default;
		inline explicit rasterize_descript(rasterize_descript const&) noexcept = default;
		inline explicit rasterize_descript(rasterize_descript&&) noexcept = default;
		inline auto operator=(rasterize_descript const&) noexcept -> rasterize_descript & = default;
		inline auto operator=(rasterize_descript&&) noexcept -> rasterize_descript & = default;
		inline ~rasterize_descript(void) noexcept = default;
	};

	class declspec_dll rasterize_state : public library::component<ID3D11RasterizerState> {
		using base = library::component<ID3D11RasterizerState>;
	public:
		inline explicit rasterize_state(ID3D11RasterizerState* component = nullptr) noexcept;
		inline explicit rasterize_state(rasterize_state const&) noexcept = default;
		inline explicit rasterize_state(rasterize_state&&) noexcept = default;
		inline auto operator=(rasterize_state const&) noexcept -> rasterize_state & = default;
		inline auto operator=(rasterize_state&&) noexcept -> rasterize_state & = default;
		inline ~rasterize_state(void) noexcept = default;
	};
}
