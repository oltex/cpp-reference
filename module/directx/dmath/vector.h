#pragma once
#include <DirectXMath.h>
#include "matrix.h"
namespace dmath {

	//typedef XMINT2 _int2;
	//typedef XMINT3 _int3;
	//typedef XMINT4 _int4;

	//typedef XMFLOAT2 _float2;


	//typedef XMINT2 _int2;
	//typedef XMINT3 _int3;
	//typedef XMINT4 _int4;

	//class int4 {
	//	DirectX::XMINT4 _int4;
	//public:
	//	inline explicit int4(void) noexcept = default;
	//	inline explicit int4(int4 const&) noexcept = default;
	//	inline explicit int4(int4&&) noexcept = default;
	//	inline auto operator=(int4 const&) noexcept -> int4 & = default;
	//	inline auto operator=(int4&&) noexcept -> int4 & = default;
	//	inline ~int4(void) noexcept = default;

	//	inline auto load(void) const noexcept -> vector {
	//		return DirectX::XMLoadInt4(&_int4);
	//	}
	//	inline void store(dmath::fvector vector) noexcept {
	//		DirectX::XMStoreInt4(&_int4, vector);
	//	}
	//};

	//using vector = DirectX::XMVECTOR;
	using fvector = DirectX::FXMVECTOR;
	struct vector {
		DirectX::XMVECTOR _vector;

		inline vector(void) noexcept = default;
		inline vector(fvector vector) noexcept
			: _vector(vector) {
		}
		inline vector(float x, float y, float z, float w) noexcept
			: _vector(DirectX::XMVectorSet(x, y, z, w)) {
		}
		inline operator DirectX::XMVECTOR(void) const noexcept {
			return _vector;
		}

		inline auto vector_add(vector value) noexcept -> vector {
			return DirectX::XMVectorAdd(_vector, value._vector);
		}
		inline auto vector_scale(float factor) noexcept -> vector {
			return DirectX::XMVectorScale(_vector, factor);
		}
		inline auto set_w(float w) noexcept -> vector {
			return DirectX::XMVectorSetW(_vector, w);
		}
		inline auto vector_normalize_2(void) noexcept -> vector {
			return DirectX::XMVector2Normalize(_vector);
		}
		inline auto vector_normalize_3(void) noexcept -> vector {
			return DirectX::XMVector3Normalize(_vector);
		}
		inline auto vector_normalize_4(void) noexcept -> vector {
			return DirectX::XMVector4Normalize(_vector);
		}

		inline auto quaternion_normalize(void) noexcept -> dmath::vector {
			return DirectX::XMQuaternionNormalize(_vector);
		}
		inline auto quaternion_multiple(fvector quaternion) noexcept -> dmath::vector {
			return DirectX::XMQuaternionMultiply(_vector, quaternion);
		}
		inline static auto quaternion_rotate_axis(fvector axis, float const angle) noexcept -> dmath::vector {
			return DirectX::XMQuaternionRotationAxis(axis, angle);
		}

		inline auto matrix_rotate_quaternion(void) noexcept -> matrix {
			return DirectX::XMMatrixRotationQuaternion(_vector);
		}
		inline auto matrix_scale(void) noexcept -> matrix {
			return DirectX::XMMatrixScalingFromVector(_vector);
		}

		inline static auto set(float x, float y, float z, float w) noexcept -> vector {
			return DirectX::XMVectorSet(x, y, z, w);
		}
		inline static auto replicate(float value) noexcept -> vector {
			return DirectX::XMVectorReplicate(value);
		}
		inline static auto multiple_add(vector v1, vector v2, vector v3) noexcept -> vector {
			return DirectX::XMVectorMultiplyAdd(v1._vector, v2._vector, v3._vector);
		}
	};



	struct float2 : public DirectX::XMFLOAT2 {
		using DirectX::XMFLOAT2::XMFLOAT2;
		inline float2(fvector vector) noexcept {
			DirectX::XMStoreFloat2(this, vector);
		}
		inline auto operator=(fvector vector) noexcept -> float2& {
			DirectX::XMStoreFloat2(this, vector);
			return *this;
		}

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat2(this);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat2(this, vector);
		}
	};
	struct float3 : public DirectX::XMFLOAT3 {
		using DirectX::XMFLOAT3::XMFLOAT3;
		using size_type = unsigned int;

		inline float3(float const(&array)[3]) noexcept
			: XMFLOAT3(array[0], array[1], array[2]) {
		}
		inline float3(fvector vector) noexcept {
			DirectX::XMStoreFloat3(this, vector);
		}
		inline auto operator=(float const(&array)[3]) noexcept -> float3& {
			x = array[0]; y = array[1]; z = array[2];
			return *this;
		}
		inline auto operator=(fvector vector) noexcept -> float3& {
			DirectX::XMStoreFloat3(this, vector);
			return *this;
		}

		inline auto operator[](size_type index) noexcept -> float& {
			return (&x)[index];
		}
		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat3(this);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat3(this, vector);
		}
	};
	struct float4 : public DirectX::XMFLOAT4 {
		using DirectX::XMFLOAT4::XMFLOAT4;
		inline float4(fvector vector) noexcept {
			DirectX::XMStoreFloat4(this, vector);
		}
		inline auto operator=(fvector vector) noexcept -> float4& {
			DirectX::XMStoreFloat4(this, vector);
			return *this;
		}

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat4(this);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat4(this, vector);
		}
	};
}

//DirectX::XMVectorSet();
//DirectX::XMQuaternionNormalize()
//XMVECTOR q = XMVectorSet(rx, ry, rz, rw);