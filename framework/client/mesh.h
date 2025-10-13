#pragma once
#include "library/container/vector.h"
#include "resource.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"
#include "graphic.h"
#include "library/tinygltf/tiny_gltf.h"

namespace framework {
	class mesh : public resource {
		using size_type = unsigned int;
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
		template<typename vertex_type, typename index_type>
		explicit mesh(library::vector<vertex_type>& vertex, library::vector<index_type>& index) noexcept {
			add_primitive(vertex, index);
		};
		explicit mesh(tinygltf::Model& model, tinygltf::Mesh& mesh) noexcept;
		explicit mesh(mesh const&) noexcept = delete;
		explicit mesh(mesh&&) noexcept = delete;
		auto operator=(mesh const&) noexcept -> mesh & = delete;
		auto operator=(mesh&&) noexcept -> mesh & = delete;
		virtual ~mesh(void) noexcept override = default;

		template<typename vertex_type, typename index_type>
		inline void add_primitive(library::vector<vertex_type>& vertex, library::vector<index_type>& index) noexcept {
			auto& primitive = _primitive.emplace_back();
			auto& device = graphic::instance()._device;
			{
				d3d11::buffer_descript descript(sizeof(vertex_type) * vertex.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0);
				d3d11::sub_resource_data data(vertex.data(), 0, 0);
				primitive._vertex_buffer = device.create_buffer(descript, &data);
				primitive._stride = sizeof(vertex_type);
			}
			{
				d3d11::buffer_descript descript(sizeof(index_type) * index.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
				d3d11::sub_resource_data data(index.data(), 0, 0);
				primitive._index_buffer = device.create_buffer(descript, &data);
				if constexpr (library::same_type<index_type, unsigned short>)
					primitive._format = DXGI_FORMAT_R16_UINT;
				primitive._count = index.size();
				//m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
		}
		void render_primitive(size_type index) const noexcept;
	};
}
