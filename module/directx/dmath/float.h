#pragma once
#include <DirectXMath.h>

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


	using vector = DirectX::XMVECTOR;
	using fvector = DirectX::FXMVECTOR;
	class float2 {
		DirectX::XMFLOAT2 _float2;
	public:
		inline explicit float2(void) noexcept = default;
		inline explicit float2(float2 const&) noexcept = default;
		inline explicit float2(float2&&) noexcept = default;
		inline auto operator=(float2 const&) noexcept -> float2 & = default;
		inline auto operator=(float2&&) noexcept -> float2 & = default;
		inline ~float2(void) noexcept = default;

		inline auto load(void) const noexcept -> vector {

			return DirectX::XMLoadFloat2(&_float2);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat2(&_float2, vector);
		}
	};
	struct float3 : public DirectX::XMFLOAT3 {
		using DirectX::XMFLOAT3::XMFLOAT3;
		//inline explicit float3(void) noexcept = default;
		//inline explicit float3(float3 const&) noexcept = default;
		//inline explicit float3(float3&&) noexcept = default;
		//inline auto operator=(float3 const&) noexcept -> float3 & = default;
		//inline auto operator=(float3&&) noexcept -> float3 & = default;
		//inline ~float3(void) noexcept = default;

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat3(this);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat3(this, vector);
		}
	};
	struct float4 : public DirectX::XMFLOAT4 {
		using DirectX::XMFLOAT4::XMFLOAT4;
		//inline explicit float4(float x, float y, float z, float w) noexcept {
		//	this->x = x;
		//	this->y = y;
		//	this->z = z;
		//	this->w = w;
		//};
		//inline explicit float4(float4 const&) noexcept = default;
		//inline explicit float4(float4&&) noexcept = default;
		//inline auto operator=(float4 const&) noexcept -> float4 & = default;
		//inline auto operator=(float4&&) noexcept -> float4 & = default;
		//inline ~float4(void) noexcept = default;

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat4(this);
		}
		inline void store(dmath::fvector vector) noexcept {
			DirectX::XMStoreFloat4(this, vector);
		}
	};

	using matrix = DirectX::XMMATRIX;
	using fmatrix = DirectX::FXMMATRIX;
	class float4x4 {
		DirectX::XMFLOAT4X4 _float4x4;
	public:
		inline explicit float4x4(void) noexcept = default;
		inline explicit float4x4(float4x4 const&) noexcept = default;
		inline explicit float4x4(float4x4&&) noexcept = default;
		inline auto operator=(float4x4 const&) noexcept -> float4x4 & = default;
		inline auto operator=(float4x4&&) noexcept -> float4x4 & = default;
		inline ~float4x4(void) noexcept = default;

		inline auto load(void) const noexcept -> matrix {
			return DirectX::XMLoadFloat4x4(&_float4x4);
		}
		inline void store(fmatrix matrix) noexcept {
			DirectX::XMStoreFloat4x4(&_float4x4, matrix);
		}
	};

	inline auto convert_to_radian(float degree) noexcept -> float {
		return DirectX::XMConvertToRadians(degree);
	}
	inline auto matrix_perspective_field_of_view_left_hand(float field_of_view, float aspect_ratio, float _near, float _far) noexcept -> matrix {
		return DirectX::XMMatrixPerspectiveFovLH(field_of_view, aspect_ratio, _near, _far);
	}
}