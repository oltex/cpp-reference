#include "vertex_shader.h"

namespace d3d11 {
	inline vertex_shader::vertex_shader(ID3D11VertexShader* component) noexcept
		: base(component) {
	}
}
