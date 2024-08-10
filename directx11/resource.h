#pragma once
#pragma comment(lib,"d3d11.lib")
#include <d3d11.h>

namespace directx11 {
	class resource {
	public:
		inline explicit resource(void) noexcept {
		}
		inline virtual ~resource(void) noexcept {
			_resource->Release();
		}
	public:
		inline auto data(void) noexcept -> ID3D11Resource& {
			return *_resource;
		}
	private:
		ID3D11Resource* _resource;
	};
}