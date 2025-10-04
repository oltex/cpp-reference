#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

namespace d3d11 {
	class declspec_dll shader_reflect : public library::component<ID3D11ShaderReflection> {
		using base = library::component<ID3D11ShaderReflection>;
	public:
		inline explicit shader_reflect(ID3D11ShaderReflection* component = nullptr) noexcept;
		inline explicit shader_reflect(shader_reflect const&) noexcept = default;
		inline explicit shader_reflect(shader_reflect&&) noexcept = default;
		inline auto operator=(shader_reflect const&) noexcept -> shader_reflect & = default;
		inline auto operator=(shader_reflect&&) noexcept -> shader_reflect & = default;
		inline ~shader_reflect(void) noexcept = default;

		inline auto get_desc(void) noexcept -> D3D11_SHADER_DESC;
		inline auto get_resource_binding_descript(unsigned int resource_index) noexcept -> D3D11_SHADER_INPUT_BIND_DESC;

	};
}