#pragma once
#include "library/container/vector.h"
#include "resource.h"
#include "texture.h"
#include "shader.h"

namespace framework {
	class material : public resource {
	public:
		library::share_pointer<shader> _shader;
		library::vector<library::share_pointer<texture>> _texture;
	public:
		explicit material(void) noexcept = default;
		explicit material(wchar_t const* const path) noexcept;
		explicit material(material const&) noexcept = delete;
		explicit material(material&&) noexcept = delete;
		auto operator=(material const&) noexcept -> material & = delete;
		auto operator=(material&&) noexcept -> material & = delete;
		virtual ~material(void) noexcept override = default;

		void set_shader(library::share_pointer<shader>& shader) noexcept;
		void add_texture(library::share_pointer<texture>& texture) noexcept;

		void set_pixel_shader_resource(d3d11::device_context& device_context) noexcept;
	};
}
