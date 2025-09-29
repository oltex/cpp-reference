#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class declspec_dll pixel_shader : public library::component<ID3D11PixelShader> {
		using base = library::component<ID3D11PixelShader>;
	public:
		inline explicit pixel_shader(ID3D11PixelShader* shader = nullptr) noexcept;
		inline explicit pixel_shader(pixel_shader const&) noexcept = default;
		inline explicit pixel_shader(pixel_shader&&) noexcept = default;
		inline auto operator=(pixel_shader const&) noexcept -> pixel_shader & = default;
		inline auto operator=(pixel_shader&&) noexcept -> pixel_shader & = default;
		inline ~pixel_shader(void) noexcept = default;
	};
}

