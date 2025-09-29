#include "sampler_state.h"

namespace d3d11 {
	inline sampler_state::sampler_state(ID3D11SamplerState* component) noexcept
		: base(component) {
	}
}
