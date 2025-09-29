#include "pixel_shader.h"

namespace d3d11 {
	inline pixel_shader::pixel_shader(ID3D11PixelShader* shader) noexcept 
		: base(shader) {
	}
}
