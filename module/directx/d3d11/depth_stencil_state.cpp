#include "depth_stencil_state.h"

namespace d3d11 {
	inline depth_stencil_state::depth_stencil_state(ID3D11DepthStencilState* component) noexcept
		: base(component) {
	}
}
