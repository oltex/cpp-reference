#include "blob.h"

namespace d3d {
	inline blob::blob(ID3DBlob* blob) noexcept
		: base(blob) {
	}
	inline auto blob::get_buffer_pointer(void) noexcept -> void* {
		return _component->GetBufferPointer();
	}
	inline auto blob::get_buffer_size(void) noexcept -> size_t {
		return _component->GetBufferSize();
	}
}
