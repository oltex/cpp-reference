#pragma once
#include "library/define.h"
#include <DirectXMath.h>

namespace dmath {
	using fmatrix = DirectX::FXMMATRIX;
	struct float4x4;
#pragma warning(suppress: 4275)
	struct declspec_dll matrix : public DirectX::XMMATRIX {
		using base = DirectX::XMMATRIX;
		using base::base;
		inline matrix(DirectX::XMMATRIX const& m) noexcept;

		inline auto store(void) noexcept -> float4x4;
		inline auto inverse(void) noexcept -> matrix;
		inline auto transpose(void) noexcept -> matrix;

		inline auto __vectorcall operator*(matrix rhs) const noexcept -> matrix {
			return base::operator*(rhs);
		}


		inline static auto identity(void) noexcept -> matrix;
		inline static auto translation(float x, float y, float z) noexcept -> matrix;
		inline static auto scaling(float x, float y, float z) noexcept -> matrix;

	};
#pragma warning(suppress: 4275)
	struct declspec_dll float4x4 : public DirectX::XMFLOAT4X4 {
		using DirectX::XMFLOAT4X4::XMFLOAT4X4;

		inline auto load(void) const noexcept -> matrix;
	};

	inline auto convert_to_radian(float degree) noexcept -> float {
		return DirectX::XMConvertToRadians(degree);
	}
	inline auto matrix_perspective_field_of_view_left_hand(float field_of_view, float aspect_ratio, float _near, float _far) noexcept -> matrix {
		return DirectX::XMMatrixPerspectiveFovLH(field_of_view, aspect_ratio, _near, _far);
	}
	//XMMatrixOrthographicLH : for 2D
}