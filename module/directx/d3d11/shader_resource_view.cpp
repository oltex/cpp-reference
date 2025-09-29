#include "shader_resource_view.h"

namespace d3d11 {
	inline shader_resource_view::shader_resource_view(ID3D11ShaderResourceView* component) noexcept
		: base(component) {
	}
}