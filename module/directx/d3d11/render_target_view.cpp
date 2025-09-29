#include "render_target_view.h"

namespace d3d11 {
	inline render_target_view::render_target_view(ID3D11RenderTargetView* component) noexcept
		: base(component) {
	}
}