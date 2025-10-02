#pragma once
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

namespace framework {
	class menu {
	public:
		explicit menu(void) noexcept = default;
		explicit menu(menu const&) noexcept = delete;
		explicit menu(menu&&) noexcept = delete;
		auto operator=(menu const&) noexcept -> menu & = delete;
		auto operator=(menu&&) noexcept -> menu & = delete;
		~menu(void) noexcept = default;

		void update(void) noexcept;
	};
}