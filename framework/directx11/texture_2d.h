#pragma once
#pragma comment(lib, "d3d11.lib")
#include "library/system/component.h"
#include "device.h"
#include <d3d11.h>

namespace directx {
	class texture_2d : public library::component<ID3D11Texture2D> {
		using base = library::component<ID3D11Texture2D>;
	public:
		inline explicit texture_2d(ID3D11Texture2D* texture) noexcept
			: base(texture) {
		}
		inline explicit texture_2d(texture_2d const&) noexcept = delete;
		inline explicit texture_2d(texture_2d&&) noexcept = default;
		inline auto operator=(texture_2d const&) noexcept -> texture_2d & = delete;
		inline auto operator=(texture_2d&&) noexcept -> texture_2d & = default;
		inline ~texture_2d(void) noexcept = default;

		inline auto render_target_view(D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr) const noexcept -> ID3D11RenderTargetView* {
			ID3D11RenderTargetView* view;
			device::instance().data()->CreateRenderTargetView(_component, desc, &view);
			return view;
		}
		inline auto shader_resource_view(D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr) const noexcept -> ID3D11ShaderResourceView* {
			ID3D11ShaderResourceView* view;
			device::instance().data()->CreateShaderResourceView(_component, desc, &view);
			return view;
		}
		inline auto unorder_access_view(D3D11_UNORDERED_ACCESS_VIEW_DESC* desc = nullptr) const noexcept -> ID3D11UnorderedAccessView* {
			ID3D11UnorderedAccessView* view;
			device::instance().data()->CreateUnorderedAccessView(_component, desc, &view);
			return view;
		}
		inline auto depth_stencil_view(D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr) const noexcept -> ID3D11DepthStencilView* {
			ID3D11DepthStencilView* view;
			device::instance().data()->CreateDepthStencilView(_component, desc, &view);
			return view;
		}
	};
}