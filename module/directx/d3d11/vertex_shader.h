#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class declspec_dll vertex_shader : public library::component<ID3D11VertexShader> {
		using base = library::component<ID3D11VertexShader>;
	public:
		inline explicit vertex_shader(ID3D11VertexShader* shader = nullptr) noexcept;
		inline explicit vertex_shader(vertex_shader const&) noexcept = default;
		inline explicit vertex_shader(vertex_shader&&) noexcept = default;
		inline auto operator=(vertex_shader const&) noexcept -> vertex_shader & = default;
		inline auto operator=(vertex_shader&&) noexcept -> vertex_shader & = default;
		inline ~vertex_shader(void) noexcept = default;
	};
}