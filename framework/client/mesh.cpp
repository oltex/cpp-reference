#include "mesh.h"
#include "graphic.h"

namespace framework {
	template<typename vertex_type, typename index_type>
	mesh::mesh(library::vector<vertex_type>& vertex, library::vector<index_type>& index) noexcept {
		auto& graphic = graphic::instance()._device;
		{
			d3d11::buffer_descript desc{};
			desc.ByteWidth = sizeof(vertex_type) * vertex.size();
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			d3d11::sub_resource_data data{};
			data.pSysMem = vertex.data();
			_vertex_buffer = graphic.create_buffer(desc, data);
			_stride = sizeof(vertex_type);
		}
		{
			d3d11::buffer_descript desc{};
			desc.ByteWidth = sizeof(index_type) * index.size();
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			d3d11::sub_resource_data data{};
			data.pSysMem = index.data();
			_index_buffer = graphic.create_buffer(desc, data);
			//m_iFace = 2;
			//m_eFormat = DXGI_FORMAT_R16_UINT;
			//m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			//m_iIndexCount = 3 * m_iFace;
		}
	}
}