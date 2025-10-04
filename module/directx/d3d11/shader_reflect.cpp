#include "shader_reflect.h"
#include <cassert>

namespace d3d11 {
	inline shader_reflect::shader_reflect(ID3D11ShaderReflection* component) noexcept
		: base(component) {
	}
	inline auto shader_reflect::get_desc(void) noexcept -> D3D11_SHADER_DESC {
		D3D11_SHADER_DESC descript;
		auto result = _component->GetDesc(&descript);
		assert(SUCCEEDED(result));
		return descript;
	}
	inline auto shader_reflect::get_resource_binding_descript(unsigned int resource_index) noexcept -> D3D11_SHADER_INPUT_BIND_DESC {
		D3D11_SHADER_INPUT_BIND_DESC descript;
		auto result = _component->GetResourceBindingDesc(resource_index, &descript);
		assert(SUCCEEDED(result));
		return descript;
	}
}