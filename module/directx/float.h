#pragma once
#include <DirectXMath.h>

namespace directx {
	//typedef XMINT2 _int2;
	//typedef XMINT3 _int3;
	//typedef XMINT4 _int4;

	//typedef XMFLOAT2 _float2;


	//typedef XMINT2 _int2;
	//typedef XMINT3 _int3;
	//typedef XMINT4 _int4;

	using vector = DirectX::XMVECTOR;
	using fvector = DirectX::FXMVECTOR;
	class float2 {
		DirectX::XMFLOAT2 _float2;
	public:
		inline explicit float2(void) noexcept = default;
		inline explicit float2(float2 const&) noexcept = delete;
		inline explicit float2(float2&&) noexcept = delete;
		inline auto operator=(float2 const&) noexcept -> float2 & = delete;
		inline auto operator=(float2&&) noexcept -> float2 & = delete;
		inline ~float2(void) noexcept = default;

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat2(&_float2);
		}
		inline void store(directx::fvector vector) noexcept {
			DirectX::XMStoreFloat2(&_float2, vector);
		}
	};
	class float3 {
		DirectX::XMFLOAT3 _float3;
	public:
		inline explicit float3(void) noexcept = default;
		inline explicit float3(float3 const&) noexcept = delete;
		inline explicit float3(float3&&) noexcept = delete;
		inline auto operator=(float3 const&) noexcept -> float3 & = delete;
		inline auto operator=(float3&&) noexcept -> float3 & = delete;
		inline ~float3(void) noexcept = default;

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat3(&_float3);
		}
		inline void store(directx::fvector vector) noexcept {
			DirectX::XMStoreFloat3(&_float3, vector);
		}
	};
	class float4 {
		DirectX::XMFLOAT4 _float4;
	public:
		inline explicit float4(void) noexcept = default;
		inline explicit float4(float4 const&) noexcept = delete;
		inline explicit float4(float4&&) noexcept = delete;
		inline auto operator=(float4 const&) noexcept -> float4 & = delete;
		inline auto operator=(float4&&) noexcept -> float4 & = delete;
		inline ~float4(void) noexcept = default;

		inline auto load(void) const noexcept -> vector {
			return DirectX::XMLoadFloat4(&_float4);
		}
		inline void store(directx::fvector vector) noexcept {
			DirectX::XMStoreFloat4(&_float4, vector);
		}
	};

	using matrix = DirectX::XMMATRIX;
	using fmatrix = DirectX::FXMMATRIX;
	class float4x4 {
		DirectX::XMFLOAT4X4 _float4x4;
	public:
		inline explicit float4x4(void) noexcept = default;
		inline explicit float4x4(float4x4 const&) noexcept = delete;
		inline explicit float4x4(float4x4&&) noexcept = delete;
		inline auto operator=(float4x4 const&) noexcept -> float4x4 & = delete;
		inline auto operator=(float4x4&&) noexcept -> float4x4 & = delete;
		inline ~float4x4(void) noexcept = default;

		inline auto load(void) const noexcept -> matrix {
			return DirectX::XMLoadFloat4x4(&_float4x4);
		}
		inline void store(fmatrix matrix) noexcept {
			DirectX::XMStoreFloat4x4(&_float4x4, matrix);
		}
	};
}