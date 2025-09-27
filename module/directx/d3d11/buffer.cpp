#include "buffer.h"

namespace d3d11 {
	inline buffer::buffer(ID3D11Buffer* buffer) noexcept
		: base(buffer) {
	}
}
