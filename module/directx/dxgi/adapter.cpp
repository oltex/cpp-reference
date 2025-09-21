#include "adapter.h"

namespace dxgi {
	inline adapter::adapter(IDXGIAdapter* component) noexcept
		: base(component) {
	};
	inline auto adapter::get_parent(IID id) noexcept -> void* {
		void* object;
		_component->GetParent(id, &object);
		return object;
	}
	inline auto adapter::get_parent_factory(void) noexcept -> dxgi::factory {
		IDXGIFactory* component = reinterpret_cast<IDXGIFactory*>(get_parent(__uuidof(IDXGIFactory)));
		return dxgi::factory(component);
	}
}