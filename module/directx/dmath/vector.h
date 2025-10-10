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
		inline vector(fvector vector) noexcept
			: _vector(vector) {
		}
		inline operator DirectX::XMVECTOR(void) const noexcept {
			return _vector;
		}

		inline auto add(vector value) noexcept -> vector {
			return DirectX::XMVectorAdd(_vector, value._vector);
		}
		inline auto scale(float factor) noexcept -> vector {
			return DirectX::XMVectorScale(_vector, factor);
		}
		inline auto set_w(float w) noexcept -> vector {
			return DirectX::XMVectorSetW(_vector, w);
		}
		inline auto normalize_3(void) noexcept -> vector {
			return DirectX::XMVector3Normalize(_vector);
		}
		inline auto quaternion_normalize(void) noexcept -> vector {
			return DirectX::XMQuaternionNormalize(_vector);
		}
		inline auto rotate_quaternion(void) noexcept -> matrix {
			return DirectX::XMMatrixRotationQuaternion(_vector);
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
		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat2(this);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat2(this, vector);
		}
	};
	struct float3 : public DirectX::XMFLOAT3 {
		using DirectX::XMFLOAT3::XMFLOAT3;

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat3(this);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat3(this, vector);
		}
	};
	struct float4 : public DirectX::XMFLOAT4 {
		using DirectX::XMFLOAT4::XMFLOAT4;

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