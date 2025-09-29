#include "blend_state.h"

namespace d3d11 {
	inline blend_state::blend_state(ID3D11BlendState* component) noexcept
		: base(component) {
	}
}
