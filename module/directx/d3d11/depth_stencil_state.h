#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct depth_stencil_descript : public D3D11_DEPTH_STENCIL_DESC {
		inline explicit depth_stencil_descript(void) noexcept = default;
		inline explicit depth_stencil_descript(depth_stencil_descript const&) noexcept = default;
		inline explicit depth_stencil_descript(depth_stencil_descript&&) noexcept = default;
		inline auto operator=(depth_stencil_descript const&) noexcept -> depth_stencil_descript & = default;
		inline auto operator=(depth_stencil_descript&&) noexcept -> depth_stencil_descript & = default;
		inline ~depth_stencil_descript(void) noexcept = default;
	};

	class declspec_dll depth_stencil_state : public library::component<ID3D11DepthStencilState> {
		using base = library::component<ID3D11DepthStencilState>;
	public:
		inline explicit depth_stencil_state(ID3D11DepthStencilState* component = nullptr) noexcept;
		inline explicit depth_stencil_state(depth_stencil_state const&) noexcept = default;
		inline explicit depth_stencil_state(depth_stencil_state&&) noexcept = default;
		inline auto operator=(depth_stencil_state const&) noexcept -> depth_stencil_state & = default;
		inline auto operator=(depth_stencil_state&&) noexcept -> depth_stencil_state & = default;
		inline ~depth_stencil_state(void) noexcept = default;
	};
}
