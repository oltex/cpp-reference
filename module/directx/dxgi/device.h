#pragma once
#include "library/system/component.h"
#include "adapter.h"
#include <dxgi.h>

namespace dxgi {
	class device : library::component<IDXGIDevice> {
		using base = library::component<IDXGIDevice>;
	public:
		inline explicit device(IDXGIDevice* component) noexcept
			: base(component) {
		};
		inline explicit device(device const&) noexcept = default;
		inline explicit device(device&&) noexcept = default;
		inline auto operator=(device const&) noexcept -> device & = default;
		inline auto operator=(device&&) noexcept -> device & = default;
		inline ~device(void) noexcept = default;

		inline auto get_parent(IID id) noexcept -> void* {
			void* parent;
			_component->GetParent(id, &parent);
			return parent;
		}
		inline auto get_parent_adapter(void) noexcept -> dxgi::adapter {
			IDXGIAdapter* component = reinterpret_cast<IDXGIAdapter*>(get_parent(__uuidof(IDXGIAdapter)));
			return dxgi::adapter(component);
		}
	};
}