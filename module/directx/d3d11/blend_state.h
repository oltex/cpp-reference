#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct blend_descript : public D3D11_BLEND_DESC {
		inline explicit blend_descript(void) noexcept = default;
		inline explicit blend_descript(blend_descript const&) noexcept = default;
		inline explicit blend_descript(blend_descript&&) noexcept = default;
		inline auto operator=(blend_descript const&) noexcept -> blend_descript & = default;
		inline auto operator=(blend_descript&&) noexcept -> blend_descript & = default;
		inline ~blend_descript(void) noexcept = default;
	};

	class declspec_dll blend_state : public library::component<ID3D11BlendState> {
		using base = library::component<ID3D11BlendState>;
	public:
		inline explicit blend_state(ID3D11BlendState* component = nullptr) noexcept;
		inline explicit blend_state(blend_state const&) noexcept = default;
		inline explicit blend_state(blend_state&&) noexcept = default;
		inline auto operator=(blend_state const&) noexcept -> blend_state & = default;
		inline auto operator=(blend_state&&) noexcept -> blend_state & = default;
		inline ~blend_state(void) noexcept = default;
	};
}
