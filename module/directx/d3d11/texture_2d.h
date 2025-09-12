#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "texture_2d_descript.h"
#include "render_target_view.h"
#include "shader_resource_view.h"
#include "unorder_access_view.h"
#include "depth_stencil_view.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class declspec_dll texture_2d : public library::component<ID3D11Texture2D> {
		using base = library::component<ID3D11Texture2D>;
	public:
		inline explicit texture_2d(texture_2d_descript descript, D3D11_SUBRESOURCE_DATA* data = nullptr) noexcept;
		inline explicit texture_2d(ID3D11Texture2D* texture) noexcept
			: base(texture) {
		}
		inline explicit texture_2d(texture_2d const&) noexcept = default;
		inline explicit texture_2d(texture_2d&&) noexcept = default;
		inline auto operator=(texture_2d const&) noexcept -> texture_2d & = default;
		inline auto operator=(texture_2d&&) noexcept -> texture_2d & = default;
		inline ~texture_2d(void) noexcept = default;

		inline auto create_render_target_view(D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr) const noexcept -> render_target_view;
		inline auto create_shader_resource_view(D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr) const noexcept -> shader_resource_view;
		inline auto create_unorder_access_view(D3D11_UNORDERED_ACCESS_VIEW_DESC* desc = nullptr) const noexcept -> unorder_access_view;
		inline auto create_depth_stencil_view(D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr) const noexcept -> depth_stencil_view;
	};
}