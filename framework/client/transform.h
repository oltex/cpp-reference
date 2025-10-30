#pragma once
#include "component.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"
#include <vector>

namespace framework {
	class transform : public componentr<transform, "transform"> {
		//library::intrusive::weak_pointer<transform, 0> _parent;
		dmath::float4x4 _float4x4;
		dmath::float4 _quaternion;
		dmath::float3 _scale;
	public:
		explicit transform(void) noexcept;
		explicit transform(std::vector<double> translation, std::vector<double> rotation, std::vector<double> scale) noexcept;
		explicit transform(transform const&) noexcept = delete;
		explicit transform(transform&&) noexcept = delete;
		auto operator=(transform const&) noexcept -> transform & = delete;
		auto operator=(transform&&) noexcept -> transform & = delete;
		virtual ~transform(void) noexcept override = default;

		virtual void edit(void) noexcept override;


		static auto buffer(void) noexcept -> d3d11::buffer&;
		auto matrix(void) noexcept -> dmath::matrix;

		//void set_parent(library::intrusive::share_pointer<transform, 0>& parent) noexcept;
		void translate(dmath::float3 move, bool local = true) noexcept;
		void rotate(dmath::float3 move, bool local = true) noexcept;

	};
}