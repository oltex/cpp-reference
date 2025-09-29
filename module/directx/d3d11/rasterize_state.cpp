#include "rasterize_state.h"

namespace d3d11 {
	inline rasterize_state::rasterize_state(ID3D11RasterizerState* component) noexcept
		: base(component) {
	}
}
