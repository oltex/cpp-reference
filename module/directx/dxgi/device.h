#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "adapter.h"
#include <dxgi.h>

namespace dxgi {
	class declspec_dll device : library::component<IDXGIDevice> {
		using base = library::component<IDXGIDevice>;
	public:
		inline explicit device(IDXGIDevice* component) noexcept;
		inline explicit device(device const&) noexcept = default;
		inline explicit device(device&&) noexcept = default;
		inline auto operator=(device const&) noexcept -> device & = default;
		inline auto operator=(device&&) noexcept -> device & = default;
		inline ~device(void) noexcept = default;

		inline auto get_parent(IID id) noexcept -> void*;
		inline auto get_parent_adapter(void) noexcept -> dxgi::adapter;
	};
}