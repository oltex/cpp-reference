#pragma once
#include "resource.h"
#include "view.h"

#include "graphic.h"

namespace directx11 {
	class render_target_view final : public view {
	public:
		inline explicit render_target_view(graphic& graphic, resource& resource) noexcept {
			auto& device = graphic.get_device();
			if (S_OK != (device.CreateRenderTargetView(&resource.data(), nullptr, reinterpret_cast<ID3D11RenderTargetView**>(&_view))))
				DebugBreak();
		}
		inline virtual ~render_target_view(void) noexcept override {
		}
	};
}