#pragma once
#pragma comment(lib,"d3d11.lib")
#include "graphic.h"
#include "window/window.h"
#include <d3d11.h>

namespace engine {
	class screen final {
	public:
		inline explicit screen(window::window& window) noexcept 
			: _device(graphic::instance().get_device()){

			{	// swap chain
				IDXGIDevice* device;
				_device.QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));
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
				if (S_OK != factory->CreateSwapChain(device, &swap_chain_desc, &_swap_chain))
					DebugBreak();
				factory->Release();
				adapter->Release();
				device->Release();
			}

			{	//render target view
				ID3D11Texture2D* texture = nullptr;
				if (S_OK != _swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture)))
					DebugBreak();
				if (S_OK != _device.CreateRenderTargetView(texture, nullptr, &_render_target_view))
					DebugBreak();
				texture->Release();
			}

			{	// depth stencil view
				ID3D11Texture2D* texture = nullptr;
				D3D11_TEXTURE2D_DESC texture_desc{};
				texture_desc.Width = sGraphic.iWidth;
				texture_desc.Height = sGraphic.iHeight;
				texture_desc.MipLevels = 1;
				texture_desc.ArraySize = 1;
				texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				texture_desc.SampleDesc.Count = 1;
				texture_desc.SampleDesc.Quality = 0;
				texture_desc.Usage = D3D11_USAGE_DEFAULT;
				texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				texture_desc.CPUAccessFlags = 0;
				texture_desc.MiscFlags = 0;
				if (S_OK != _device.CreateTexture2D(&texture_desc, nullptr, &texture))
					DebugBreak();
				if (S_OK != _device.CreateDepthStencilView(texture, nullptr, &_depth_stencil_view))
					DebugBreak();
				texture->Release();
			}
		}
		inline ~screen(void) noexcept {
			_swap_chain->Release();
		}
	public:
		inline void set_render_traget(void) noexcept {
			m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
			return S_OK;
		}
	private:
		ID3D11Device& _device;
		ID3D11DeviceContext& _context;

		IDXGISwapChain* _swap_chain;
		ID3D11RenderTargetView* _render_target_view;
		ID3D11DepthStencilView* _depth_stencil_view;
	};
}