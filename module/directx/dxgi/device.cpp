#include "device.h"

namespace dxgi {
	inline device::device(IDXGIDevice* component) noexcept
		: base(component) {
	};
	inline auto device::get_parent(IID id) noexcept -> void* {
		void* parent;
		_component->GetParent(id, &parent);
		return parent;
	}
	inline auto device::get_parent_adapter(void) noexcept -> dxgi::adapter {
		IDXGIAdapter* component = reinterpret_cast<IDXGIAdapter*>(get_parent(__uuidof(IDXGIAdapter)));
		return dxgi::adapter(component);
	}
}