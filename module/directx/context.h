#pragma once
#include "library/system/component.h"
#include "render_target_view.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace directx {
	class context : public library::component<ID3D11DeviceContext> {
		using base = library::component<ID3D11DeviceContext>;
	public:
		inline explicit context(ID3D11DeviceContext* context) noexcept
			: base(context) {
		}
		inline explicit context(context const&) noexcept = delete;
		inline explicit context(context&&) noexcept = default;
		inline auto operator=(context const&) noexcept -> context & = delete;
		inline auto operator=(context&&) noexcept -> context & = default;
		inline ~context(void) noexcept = default;

		inline auto clear_render_target_view(render_target_view& rtv, ) {
			_component->ClearRenderTargetView(rtv.data(),)
		}
	};
}