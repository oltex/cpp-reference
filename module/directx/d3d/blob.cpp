#include "blob.h"
#include <cassert>

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

	inline auto blob::reflect(void) noexcept -> d3d11::shader_reflect {
		ID3D11ShaderReflection* component;
		auto result = D3DReflect(_component->GetBufferPointer(), _component->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&component);
		assert(SUCCEEDED(result));
		return d3d11::shader_reflect(component);
	}


	//ComPtr<ID3D11ShaderReflection> r;
	//D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(),
	//	IID_ID3D11ShaderReflection, (void**)&r);
}
