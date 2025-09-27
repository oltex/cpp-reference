#pragma once
#include "resource.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class mesh : public resource {
		d3d11::buffer _buffer;
	public:
		explicit mesh(void) noexcept;
		explicit mesh(mesh const&) noexcept = delete;
		explicit mesh(mesh&&) noexcept = delete;
		auto operator=(mesh const&) noexcept -> mesh & = delete;
		auto operator=(mesh&&) noexcept -> mesh & = delete;
		virtual ~mesh(void) noexcept override = default;
	};
}