#include "matrix.h"

namespace dmath {
	inline matrix::matrix(fmatrix matrix) noexcept
		: DirectX::XMMATRIX(matrix) {
	}
	inline matrix::matrix(float4x4 const& float4x4) noexcept
		: DirectX::XMMATRIX(DirectX::XMLoadFloat4x4(&float4x4)) {
	}
	inline auto matrix::operator=(float4x4 const& float4x4) noexcept -> matrix& {
		*this = DirectX::XMLoadFloat4x4(&float4x4);
		return *this;
	}
	inline auto matrix::store(void) noexcept -> float4x4 {
		float4x4 _float;
		DirectX::XMStoreFloat4x4(&_float, *this);
		return _float;
	}
	inline auto matrix::inverse(void) noexcept -> matrix {
		return DirectX::XMMatrixInverse(nullptr, *this);
	}
	inline auto matrix::transpose(void) noexcept -> matrix {
		return DirectX::XMMatrixTranspose(*this);
	}
	inline auto matrix::identity(void) noexcept -> matrix {
		return DirectX::XMMatrixIdentity();
	}
	inline auto matrix::translation(float x, float y, float z) noexcept -> matrix {
		return DirectX::XMMatrixTranslation(x, y, z);
	}
	inline auto matrix::scaling(float x, float y, float z) noexcept -> matrix {
		return DirectX::XMMatrixScaling(x, y, z);
	}

	inline float4x4::float4x4(fmatrix matrix) noexcept {
		DirectX::XMStoreFloat4x4(this, matrix);
	}
	inline auto float4x4::operator=(fmatrix matrix) noexcept -> float4x4& {
		DirectX::XMStoreFloat4x4(this, matrix);
		return *this;
	}
	inline auto float4x4::load(void) const noexcept -> matrix {
		return static_cast<matrix>(DirectX::XMLoadFloat4x4(this));
	}
}
