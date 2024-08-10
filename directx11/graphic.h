#pragma once
#pragma comment(lib,"d3d11.lib")

#include "design-pattern/singleton.h"
#include "window/window.h"
#include "swap_chain.h"

#include <d3d11.h>

namespace directx11 {
	class graphic final : public singleton<graphic> {
		friend class singleton<graphic>;
	private:
		inline explicit graphic(void) noexcept {
			unsigned int flag = 0;
#ifdef _DEBUG
			flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL freature_level{};
			if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, nullptr, 0, D3D11_SDK_VERSION, &_device, &freature_level, &_context))
				DebugBreak();
		}
		inline ~graphic(void) noexcept {
			_context->Release();
			_device->Release();
		}
	public:
		inline auto create_swap_chain(window::window window, unsigned int const width, unsigned int const height) noexcept {
			IDXGIDevice* device;
			_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));
			IDXGIAdapter* adapter;
			device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));
			IDXGIFactory* factory;
			adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
			DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
			swap_chain_desc.BufferDesc.Width = width;
			swap_chain_desc.BufferDesc.Height = height;
			swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swap_chain_desc.SampleDesc.Quality = 0;
			swap_chain_desc.SampleDesc.Count = 1;
			swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_desc.BufferCount = 1;
			swap_chain_desc.OutputWindow = window.data();
			swap_chain_desc.Windowed = true; //전체화면 모드 어떻게 얻는지 확인필요함
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			IDXGISwapChain* swapchain;
			if (S_OK != factory->CreateSwapChain(_device, &swap_chain_desc, &swapchain))
				DebugBreak();
			factory->Release();
			adapter->Release();
			device->Release();

			return swap_chain(swapchain);
		}
	public:
		inline auto get_device(void) noexcept -> ID3D11Device& {
			return *_device;
		}
		inline auto get_context(void) noexcept -> ID3D11DeviceContext& {
			return *_context;
		}
	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;

	};
}


//#include <DirectXMath.h>
//#include <DirectXPackedVector.h>
//#include <DirectXCollision.h>
//#include <d3dcompiler.h>
//d3d11.lib
//dinput8.lib
//dxguid.lib
//Effects11d.lib
//DirectXTKd.lib
//assimp - vc143 - mtd.lib
//fmod_vc.lib
//fmodL_vc.lib
