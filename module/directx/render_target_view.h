#pragma once
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace directx {
	class render_target_view : public library::component<ID3D11RenderTargetView> {
		using base = library::component<ID3D11RenderTargetView>;
	public:
		inline explicit render_target_view(ID3D11RenderTargetView* render_target_view) noexcept
			: base(render_target_view) {
		}
		inline explicit render_target_view(render_target_view const&) noexcept = delete;
		inline explicit render_target_view(render_target_view&&) noexcept = default;
		inline auto operator=(render_target_view const&) noexcept -> render_target_view & = delete;
		inline auto operator=(render_target_view&&) noexcept -> render_target_view & = default;
		inline ~render_target_view(void) noexcept = default;
	};
}