#include "depth_stencil_view.h"

namespace d3d11 {
	inline depth_stencil_view::depth_stencil_view(ID3D11DepthStencilView* component) noexcept
		: base(component) {
	}
}