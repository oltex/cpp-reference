#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct sampler_descript : public D3D11_SAMPLER_DESC {
		inline explicit sampler_descript(void) noexcept = default;
		inline explicit sampler_descript(sampler_descript const&) noexcept = default;
		inline explicit sampler_descript(sampler_descript&&) noexcept = default;
		inline auto operator=(sampler_descript const&) noexcept -> sampler_descript & = default;
		inline auto operator=(sampler_descript&&) noexcept -> sampler_descript & = default;
		inline ~sampler_descript(void) noexcept = default;
	};

	class declspec_dll sampler_state : public library::component<ID3D11SamplerState> {
		using base = library::component<ID3D11SamplerState>;
	public:
		inline explicit sampler_state(ID3D11SamplerState* component = nullptr) noexcept;
		inline explicit sampler_state(sampler_state const&) noexcept = default;
		inline explicit sampler_state(sampler_state&&) noexcept = default;
		inline auto operator=(sampler_state const&) noexcept -> sampler_state & = default;
		inline auto operator=(sampler_state&&) noexcept -> sampler_state & = default;
		inline ~sampler_state(void) noexcept = default;
	};
}