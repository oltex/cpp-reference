#pragma once
#include "library/define.h"
#include "library/system/component.h"
//#include "library/pattern/singleton.h"
#include "../dxgi/device.h"
#include "texture_2d.h"
#include "device_context.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class declspec_dll device : public library::component<ID3D11Device>/*, public library::singleton<device, true, true>*/ {
		//friend class library::singleton<device, true, true>;
		using base = library::component<ID3D11Device>;
	public:
		inline explicit device(D3D_DRIVER_TYPE driver_type, unsigned int flag) noexcept;
		inline explicit device(device const&) noexcept = delete;
		inline explicit device(device&&) noexcept = delete;
		inline auto operator=(device const&) noexcept -> device & = delete;
		inline auto operator=(device&&) noexcept -> device & = delete;
		inline ~device(void) noexcept = default;

		inline auto get_immediate_context(void) const noexcept -> device_context;
		inline auto create_deferred_context(void) const noexcept -> device_context;
		inline auto query_interface_dxgi_device(void) noexcept -> dxgi::device;

		inline auto create_texture_2d(texture_2d_descript descript, D3D11_SUBRESOURCE_DATA* data = nullptr) noexcept -> texture_2d;
		inline void create_texture_from_file(wchar_t const* const path, shader_resource_view* srv) noexcept;

		inline auto create_render_target_view(texture_2d& texture, D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr) const noexcept -> render_target_view;
		inline auto create_shader_resource_view(texture_2d& texture, D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr) const noexcept -> shader_resource_view;
		inline auto create_unorder_access_view(texture_2d& texture, D3D11_UNORDERED_ACCESS_VIEW_DESC* desc = nullptr) const noexcept -> unorder_access_view;
		inline auto create_depth_stencil_view(texture_2d& texture, D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr) const noexcept -> depth_stencil_view;
	};
}