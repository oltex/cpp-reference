#include "editor.h"
#include "window.h"
#include "graphic.h"

namespace framework {
	editor::editor(void) noexcept {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		auto& window = window::instance();
		ImGui_ImplWin32_Init(window.data());
		auto& graphic = graphic::instance();
		ImGui_ImplDX11_Init(graphic._device.data(), graphic._device_context.data());
	}
	editor::~editor(void) noexcept {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void editor::update(void) noexcept {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		_menu.update();
	}
	void editor::render(void) noexcept {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//ImGuiIO& io = ImGui::GetIO(); (void)io;
		//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		//	ImGui::UpdatePlatformWindows();
		//	ImGui::RenderPlatformWindowsDefault();
		//}
	}
}
