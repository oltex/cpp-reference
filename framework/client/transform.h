#pragma once
#include "component.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class transform : public component, public componentr<transform, "transform"> {
	public:
		dmath::float4x4 _float4x4;
	public:
		explicit transform(void) noexcept;
		explicit transform(transform const&) noexcept = delete;
		explicit transform(transform&&) noexcept = delete;
		auto operator=(transform const&) noexcept -> transform & = delete;
		auto operator=(transform&&) noexcept -> transform & = delete;
		virtual ~transform(void) noexcept override = default;
	};
}