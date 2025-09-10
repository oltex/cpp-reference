#pragma once
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace directx {
	class shader_resource_view : public library::component<ID3D11ShaderResourceView> {
		using base = library::component<ID3D11ShaderResourceView>;
	public:
		inline explicit shader_resource_view(ID3D11ShaderResourceView* shader_resource_view) noexcept
			: base(shader_resource_view) {
		}
		inline explicit shader_resource_view(shader_resource_view const&) noexcept = delete;
		inline explicit shader_resource_view(shader_resource_view&&) noexcept = default;
		inline auto operator=(shader_resource_view const&) noexcept -> shader_resource_view & = delete;
		inline auto operator=(shader_resource_view&&) noexcept -> shader_resource_view & = default;
		inline ~shader_resource_view(void) noexcept = default;
	};
}