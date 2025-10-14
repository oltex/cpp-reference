#pragma once
#include "editor.h"

namespace framework {
	class menu : public editor {
	public:
		explicit menu(void) noexcept = default;
		explicit menu(menu const&) noexcept = delete;
		explicit menu(menu&&) noexcept = delete;
		auto operator=(menu const&) noexcept -> menu & = delete;
		auto operator=(menu&&) noexcept -> menu & = delete;
		virtual ~menu(void) noexcept override = default;

		virtual void update(void) noexcept override;

		void open_gltf(char const* const path) noexcept;
		void open_image(char const* const path) noexcept;
	};
}