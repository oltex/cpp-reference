#pragma once
#pragma comment(lib,"d3d11.lib")
#include <d3d11.h>

namespace directx11 {
	class view {
	public:
		inline explicit view(void) noexcept {
		}
		inline virtual ~view(void) noexcept {
			_view->Release();
		}
	protected:
		ID3D11View* _view;
	};
}