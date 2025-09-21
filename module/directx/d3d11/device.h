#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "library/pattern/singleton.h"
#include "../dxgi/device.h"
#include "texture_2d.h"
#include "device_context.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class declspec_dll device : library::component<ID3D11Device>, public library::singleton<device, true, true> {
		friend class library::singleton<device, true, true>;
		using base = library::component<ID3D11Device>;

		inline explicit device(D3D_DRIVER_TYPE driver_type, unsigned int flag) noexcept;
		inline explicit device(device const&) noexcept = delete;
		inline explicit device(device&&) noexcept = delete;
		inline auto operator=(device const&) noexcept -> device & = delete;
		inline auto operator=(device&&) noexcept -> device & = delete;
		inline ~device(void) noexcept = default;
	public:
		inline auto get_immediate_context(void) const noexcept -> device_context;
		inline auto create_deferred_context(void) const noexcept -> device_context;
		inline auto query_interface_dxgi_device(void) noexcept -> dxgi::device;
	};
}