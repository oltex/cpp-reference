#pragma once
#include "component.h"
#include "mesh.h"

namespace framework {
	class renderer : public component {
		library::share_pointer<mesh> _mesh;
	public:
		explicit renderer(void) noexcept;
		explicit renderer(renderer const&) noexcept = delete;
		explicit renderer(renderer&&) noexcept = delete;
		auto operator=(renderer const&) noexcept -> renderer & = delete;
		auto operator=(renderer&&) noexcept -> renderer & = delete;
		virtual ~renderer(void) noexcept override = default;
	};
}