#pragma once
#pragma comment(lib,"d3d11.lib")
#include "design-pattern/singleton.h"
#include <d3d11.h>

namespace engine {
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
