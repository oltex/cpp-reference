#pragma once
#include "library/container/vector.h"
#include "resource.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"
#include "graphic.h"

namespace framework {
	class mesh : public resource {
	public:
		struct primitive {
			d3d11::buffer _vertex_buffer;
			unsigned int _vertex_stride;
			d3d11::buffer _index_buffer;
			unsigned int _index_size;
			DXGI_FORMAT _index_format;
		};
		library::vector<primitive> _primitive;

		d3d11::buffer _vertex_buffer;
		unsigned int _stride;
		d3d11::buffer _index_buffer;
		DXGI_FORMAT _format;
		unsigned int _index_count;
	public:
		template<typename vertex_type, typename index_type>
		explicit mesh(library::vector<vertex_type>& vertex, library::vector<index_type>& index) noexcept {
			auto& device = graphic::instance()._device;
			{
				d3d11::buffer_descript desc(sizeof(vertex_type) * vertex.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0);
				d3d11::sub_resource_data data(vertex.data(), 0, 0);
				_vertex_buffer = device.create_buffer(desc, &data);
				_stride = sizeof(vertex_type);
			}
			{
				d3d11::buffer_descript desc(sizeof(index_type) * index.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
				d3d11::sub_resource_data data(index.data(), 0, 0);
				_index_buffer = device.create_buffer(desc, &data);
				_format = DXGI_FORMAT_R16_UINT;
				_index_count = index.size();
				//m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
		}
		explicit mesh(char const* const path) noexcept;
		explicit mesh(mesh const&) noexcept = delete;
		explicit mesh(mesh&&) noexcept = delete;
		auto operator=(mesh const&) noexcept -> mesh & = delete;
		auto operator=(mesh&&) noexcept -> mesh & = delete;
		virtual ~mesh(void) noexcept override = default;
	};
}
