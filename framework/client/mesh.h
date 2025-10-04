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
			unsigned int _stride;
			d3d11::buffer _index_buffer;
			unsigned int _count;
			DXGI_FORMAT _format;
		};
		library::vector<primitive> _primitive;
	public:
		explicit mesh(void) noexcept = default;
		explicit mesh(char const* const path) noexcept;
		explicit mesh(mesh const&) noexcept = delete;
		explicit mesh(mesh&&) noexcept = delete;
		auto operator=(mesh const&) noexcept -> mesh & = delete;
		auto operator=(mesh&&) noexcept -> mesh & = delete;
		virtual ~mesh(void) noexcept override = default;

		template<typename vertex_type, typename index_type>
		inline void add_primitive(library::vector<vertex_type>& vertex, library::vector<index_type>& index, DXGI_FORMAT format) {
			auto& device = graphic::instance()._device;
			auto& primitive = _primitive.emplace_back();
			{
				d3d11::buffer_descript descript(sizeof(vertex_type) * vertex.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0);
				d3d11::sub_resource_data data(vertex.data(), 0, 0);
				primitive._vertex_buffer = device.create_buffer(descript, &data);
				primitive._stride = sizeof(vertex_type);
			}
			{
				d3d11::buffer_descript desc(sizeof(index_type) * index.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
				d3d11::sub_resource_data data(index.data(), 0, 0);
				primitive._index_buffer = device.create_buffer(desc, &data);
				primitive._format = format;
				primitive._count = index.size();
				//m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
		}
	};
}
