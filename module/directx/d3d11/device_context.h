#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "render_target_view.h"
#include "depth_stencil_view.h"
#include "view_port.h"

#include "input_layout.h"
#include "vertex_shader.h"
#include "pixel_shader.h"

#include "buffer.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class buffer;
	class texture_2d;
	struct map_sub_resource : public D3D11_MAPPED_SUBRESOURCE {
	};

	class declspec_dll device_context : public library::component<ID3D11DeviceContext> {
		using base = library::component<ID3D11DeviceContext>;
	public:
		inline explicit device_context(ID3D11DeviceContext* component = nullptr) noexcept;
		inline explicit device_context(device_context const&) noexcept = default;
		inline explicit device_context(device_context&&) noexcept = default;
		inline auto operator=(device_context const&) noexcept -> device_context & = default;
		inline auto operator=(device_context&&) noexcept -> device_context & = default;
		inline ~device_context(void) noexcept = default;

		inline void save_texture_from_file(texture_2d const& texture, wchar_t const* const path) noexcept;

		inline void set_view_port(unsigned int number, view_port* view_port) noexcept;
		inline void set_view_port(view_port& view_port) noexcept;
		//inline auto get_view_port(void) noexcept -> view_port;
		inline void set_render_target(render_target_view& rtv, depth_stencil_view& dsv) noexcept;
		inline void clear_render_target_view(render_target_view& rtv, float color[4]) noexcept;
		inline void clear_depth_stencil_view(depth_stencil_view& dsv, unsigned int clear_flag, float depth, unsigned char stencil) noexcept;

		inline void set_input_layout(input_layout& input_layout) noexcept;
		inline void set_vertex_shader(vertex_shader& vertex_shader) noexcept;
		inline void set_vertex_shader_constant_buffer(unsigned int start_slot, unsigned int number_buffer, ID3D11Buffer* constant_buffer[]) noexcept;
		inline void set_pixel_shader(pixel_shader& pixel_shader) noexcept;
		inline void set_pixel_shader_resource(unsigned int start_slot, unsigned int number_view, ID3D11ShaderResourceView* shader_resource_view[]) noexcept;
		inline void set_pixel_shader_sampler(unsigned int start_slot, unsigned int number_sampler, ID3D11SamplerState* sampler_state[]) noexcept;

		inline void set_vertex_buffer(unsigned int start_slot, unsigned int number_buffer, ID3D11Buffer* vertex_buffer[], unsigned int stride[], unsigned int offset[]) noexcept;
		inline void set_index_buffer(buffer& index_buffer, DXGI_FORMAT format, unsigned int offset) noexcept;

		inline auto map(ID3D11Resource* resource, unsigned int sub_resource, D3D11_MAP type, unsigned int flag) noexcept -> map_sub_resource;
		inline void unmap(ID3D11Resource* resource, unsigned int sub_resource) noexcept;
		inline void set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept;
		inline void draw_index(unsigned int index_count, unsigned int start_index, unsigned int base_vertex) noexcept;
	};
}