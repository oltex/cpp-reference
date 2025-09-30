#include "shader.h"
#include "graphic.h"

namespace framework {
	shader::shader(wchar_t const* const path, char const* const entry, char const* const target,
		wchar_t const* const pixel_path, char const* const pixel_entry, char const* const pixel_target) noexcept {
		auto device = graphic::instance()._device;
		{
			auto blob = d3d::complie_from_file(path, entry, target);
			_input_layout = device.create_input_layout(blob);
			_vertex_shader = device.create_vertex_shader(blob);
		}
		{
			auto blob = d3d::complie_from_file(pixel_path, pixel_entry, pixel_target);
			_pixel_shader = device.create_pixel_shader(blob);
		}
	}
}
