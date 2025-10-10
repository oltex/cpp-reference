#pragma once
#include "component.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"
#include <vector>

namespace framework {
	class transform : public component, public componentr<transform, "transform"> {
	public:
		library::intrusive::weak_pointer<transform, 0> _parent;
		dmath::float4x4 _float4x4;
	public:
		explicit transform(void) noexcept;
		explicit transform(std::vector<double> translation, std::vector<double> rotation, std::vector<double> scale) noexcept;
		explicit transform(transform const&) noexcept = delete;
		explicit transform(transform&&) noexcept = delete;
		auto operator=(transform const&) noexcept -> transform & = delete;
		auto operator=(transform&&) noexcept -> transform & = delete;
		virtual ~transform(void) noexcept override = default;

		void set_parent(library::intrusive::share_pointer<transform, 0>& parent) noexcept;
		void translate(dmath::float3 move) noexcept;
	};
}