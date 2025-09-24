#include "texture_2d.h"
#include "device.h"

namespace d3d11 {
	inline texture_2d::texture_2d(ID3D11Texture2D* texture) noexcept
		: base(texture) {
	}

}