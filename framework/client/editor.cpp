#include "editor.h"
#include "window.h"
#include "graphic.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

#include "menu.h"
#include "asset.h"

namespace framework {
	editors::editors(void) noexcept {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		auto& window = window::instance();
		ImGui_ImplWin32_Init(window.data());
		auto& graphic = graphic::instance();
		ImGui_ImplDX11_Init(graphic._device.data(), graphic._device_context.data());

		_editor.emplace_back(std::move(library::make_unique<menu>()));
		_editor.emplace_back(std::move(library::make_unique<asset>()));
	}
	editors::~editors(void) noexcept {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void editors::update(void) noexcept {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		for (auto& editor : _editor) {
			editor->update();
		}
	}
	void editors::render(void) noexcept {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
