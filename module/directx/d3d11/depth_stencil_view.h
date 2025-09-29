#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class declspec_dll depth_stencil_view : public library::component<ID3D11DepthStencilView> {
		using base = library::component<ID3D11DepthStencilView>;
	public:
		inline explicit depth_stencil_view(ID3D11DepthStencilView* component = nullptr) noexcept;
		inline explicit depth_stencil_view(depth_stencil_view const&) noexcept = default;
		inline explicit depth_stencil_view(depth_stencil_view&&) noexcept = default;
		inline auto operator=(depth_stencil_view const&) noexcept -> depth_stencil_view & = default;
		inline auto operator=(depth_stencil_view&&) noexcept -> depth_stencil_view & = default;
		inline ~depth_stencil_view(void) noexcept = default;
	};
}