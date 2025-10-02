#pragma once
#include "library/pattern/singleton.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

#include "menu.h"

namespace framework {
	class editor : public library::singleton<editor> {
		friend class library::singleton<editor>;

		menu _menu;

		explicit editor(void) noexcept;
		explicit editor(editor const&) noexcept = delete;
		explicit editor(editor&&) noexcept = delete;
		auto operator=(editor const&) noexcept -> editor & = delete;
		auto operator=(editor&&) noexcept -> editor & = delete;
		~editor(void) noexcept;
	public:
		void update(void) noexcept;
		void render(void) noexcept;
	};
}