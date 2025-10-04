#pragma once
#include "library/container/vector.h"
#include "resource.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"
#include "graphic.h"
#include "texture.h"

namespace framework {
	class material : public resource {
		library::vector<library::share_pointer<texture>> _texture;
	public:
		explicit material(void) noexcept = default;
		explicit material(wchar_t const* const path) noexcept;
		explicit material(material const&) noexcept = delete;
		explicit material(material&&) noexcept = delete;
		auto operator=(material const&) noexcept -> material & = delete;
		auto operator=(material&&) noexcept -> material & = delete;
		virtual ~material(void) noexcept override = default;
	};
}
