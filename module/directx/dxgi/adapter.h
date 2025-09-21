#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "factory.h"
#include <dxgi.h>

namespace dxgi {
	class declspec_dll adapter : library::component<IDXGIAdapter> {
		using base = library::component<IDXGIAdapter>;
	public:
		inline explicit adapter(IDXGIAdapter* component) noexcept;
		inline explicit adapter(adapter const&) noexcept = default;
		inline explicit adapter(adapter&&) noexcept = default;
		inline auto operator=(adapter const&) noexcept -> adapter & = default;
		inline auto operator=(adapter&&) noexcept -> adapter & = default;
		inline ~adapter(void) noexcept = default;

		inline auto get_parent(IID id) noexcept -> void*;
		inline auto get_parent_factory(void) noexcept -> dxgi::factory;
	};
}