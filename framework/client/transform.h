#pragma once
#include "component.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class transform : public component {
		dmath::float4x4 _float4x4;
	public:
		inline explicit transform(void) noexcept = default;
		inline explicit transform(transform const&) noexcept = delete;
		inline explicit transform(transform&&) noexcept = delete;
		inline auto operator=(transform const&) noexcept -> transform & = delete;
		inline auto operator=(transform&&) noexcept -> transform & = delete;
		inline virtual ~transform(void) noexcept override = default;
	};
}