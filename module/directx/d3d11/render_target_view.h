#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct render_target_view_descript : public D3D11_RENDER_TARGET_VIEW_DESC {
	};

	class declspec_dll render_target_view : public library::component<ID3D11RenderTargetView> {
		using base = library::component<ID3D11RenderTargetView>;
	public:
		inline explicit render_target_view(ID3D11RenderTargetView* component = nullptr) noexcept;
		inline explicit render_target_view(render_target_view const&) noexcept = default;
		inline explicit render_target_view(render_target_view&&) noexcept = default;
		inline auto operator=(render_target_view const&) noexcept -> render_target_view & = default;
		inline auto operator=(render_target_view&&) noexcept -> render_target_view & = default;
		inline ~render_target_view(void) noexcept = default;
	};
}