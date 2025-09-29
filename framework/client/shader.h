#pragma once
#include "resource.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class shader : public resource {
		d3d::blob _blob;
		d3d11::input_layout _input_layout;
	public:
		explicit shader(wchar_t const* const path, char const* const entry, char const* const target) noexcept;
		explicit shader(shader const&) noexcept = delete;
		explicit shader(shader&&) noexcept = delete;
		auto operator=(shader const&) noexcept -> shader & = delete;
		auto operator=(shader&&) noexcept -> shader & = delete;
		virtual ~shader(void) noexcept override = default;
	};
}