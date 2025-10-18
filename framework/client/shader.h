#pragma once
#include "resource.h"
#include "library/container/vector.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class shader : public resourcer<shader, "shader"> {
	public:
		d3d11::input_layout _input_layout;
		d3d11::vertex_shader _vertex_shader;
		d3d11::pixel_shader _pixel_shader;
	public:
		explicit shader(wchar_t const* const path, wchar_t const* const pixel_path, library::vector<d3d11::input_element_descript>& descript) noexcept;
		explicit shader(shader const&) noexcept = delete;
		explicit shader(shader&&) noexcept = delete;
		auto operator=(shader const&) noexcept -> shader & = delete;
		auto operator=(shader&&) noexcept -> shader & = delete;
		virtual ~shader(void) noexcept override = default;

		auto get_input_layout(void) noexcept -> d3d11::input_layout;
	};
}