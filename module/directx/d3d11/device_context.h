#pragma once
#include "library/system/component.h"
#include "render_target_view.h"
#include "depth_stencil_view.h"
#include "view_port.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class device_context : public library::component<ID3D11DeviceContext> {
		using base = library::component<ID3D11DeviceContext>;
	public:
		inline explicit device_context(ID3D11DeviceContext* device_context) noexcept
			: base(device_context) {
		}
		inline explicit device_context(device_context const&) noexcept = default;
		inline explicit device_context(device_context&&) noexcept = default;
		inline auto operator=(device_context const&) noexcept -> device_context & = default;
		inline auto operator=(device_context&&) noexcept -> device_context & = default;
		inline ~device_context(void) noexcept = default;

		inline auto clear_render_target_view(render_target_view& rtv, float color[4]) noexcept {
			_component->ClearRenderTargetView(rtv.data(), color);
		}
		inline auto set_render_target(render_target_view& rtv, depth_stencil_view& dsv) noexcept {
			_component->OMGetRenderTargets(1, &rtv.data(), &dsv.data());
		}
		inline void set_view_port(unsigned int number, view_port* view_port) noexcept {
			_component->RSSetViewports(number, view_port);
		}
		inline void set_view_port(view_port& view_port) noexcept {
			_component->RSSetViewports(1, &view_port);
		}
		inline auto get_view_port(void) noexcept -> view_port {
			unsigned int count;
			_component->RSGetViewports(&count, nullptr);
		}

		//m_pContext->RSSetViewports(1, &Viewport);

	};
}