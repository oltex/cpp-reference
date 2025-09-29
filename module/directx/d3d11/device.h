#pragma once
//#include "library/pattern/singleton.h"
#include "library/define.h"
#include "library/system/component.h"
#include "library/container/vector.h"
#include "../dxgi/device.h"
#include "../d3d/blob.h"
#include "device_context.h"

#include "input_layout.h"
#include "rasterize_state.h"
#include "blend_state.h"
#include "depth_stencil_state.h"

#include "texture_2d.h"
#include "buffer.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct sub_resource_data : D3D11_SUBRESOURCE_DATA {
	};
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

		inline auto create_input_layout(library::vector<input_element_descript>& desc, d3d::blob& blob) const noexcept -> input_layout;
		inline auto create_rasterize_state(rasterize_descript& desc) const noexcept -> rasterize_state;
		inline auto create_blend_state(blend_descript& desc) const noexcept -> blend_state;
		inline auto create_depth_stencil_state(depth_stencil_descript& desc) const noexcept -> depth_stencil_state;

		inline auto create_buffer(buffer_descript const& desc, sub_resource_data const& data) const noexcept -> buffer;
		inline auto create_texture_2d(texture_2d_descript& desc, sub_resource_data* data = nullptr) noexcept -> texture_2d;
		inline void create_texture_from_file(wchar_t const* const path, shader_resource_view* srv) noexcept;
		inline auto create_render_target_view(texture_2d& texture, D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr) const noexcept -> render_target_view;
		inline auto create_shader_resource_view(texture_2d& texture, D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr) const noexcept -> shader_resource_view;
		inline auto create_unorder_access_view(texture_2d& texture, D3D11_UNORDERED_ACCESS_VIEW_DESC* desc = nullptr) const noexcept -> unorder_access_view;
		inline auto create_depth_stencil_view(texture_2d& texture, D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr) const noexcept -> depth_stencil_view;
	};
}
//RT / DSV + Viewport ->
//IA(InputLayout + primitive topology) / VS / PS -> 
//RS / DSS / BS / Sampler -> 
//CB / SRV / UAV / VB / IB -> 
//draw 


//ID3D11DepthStencilState* MakeDSS_Default(ID3D11Device*);
//ID3D11SamplerState* MakeSamp_LinearWrap(ID3D11Device*);
