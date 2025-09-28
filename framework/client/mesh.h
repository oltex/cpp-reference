#pragma once
#include "library/container/vector.h"
#include "resource.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class mesh : public resource {
		d3d11::buffer _vertex_buffer;
		unsigned int _stride;

		d3d11::buffer _index_buffer;
	public:
		template<typename vertex_type, typename index_type>
		explicit mesh(library::vector<vertex_type>& vertex, library::vector<index_type>& index) noexcept;
		explicit mesh(mesh const&) noexcept = delete;
		explicit mesh(mesh&&) noexcept = delete;
		auto operator=(mesh const&) noexcept -> mesh & = delete;
		auto operator=(mesh&&) noexcept -> mesh & = delete;
		virtual ~mesh(void) noexcept override = default;
	};
}