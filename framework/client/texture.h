#pragma once
#include "resource.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class texture : public resource {
		d3d11::shader_resource_view _srv;
	public:
		explicit texture(wchar_t const* const path) noexcept;
		inline explicit texture(texture const&) noexcept;
		inline explicit texture(texture&&) noexcept = delete;
		inline auto operator=(texture const&) noexcept -> texture & = delete;
		inline auto operator=(texture&&) noexcept -> texture & = delete;
		inline virtual ~texture(void) noexcept override;
	};
}