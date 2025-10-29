#include "editor.h"
#include "window.h"
#include "graphic.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"
#include "menu.h"

namespace framework {
	editors::editors(void) noexcept {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", 18.0f);

		auto& window = window::instance();
		ImGui_ImplWin32_Init(window.data());
		auto& graphic = graphic::instance();
		ImGui_ImplDX11_Init(graphic._device.data(), graphic._device_context.data());

		auto& style = ImGui::GetStyle();
		style.WindowBorderSize = 1.f;
		style.TabBarBorderSize = 0.f;
		style.DockingSeparatorSize = 1.f;
		style.TabBarOverlineSize = 1.f;
		style.WindowRounding = 4.f;
		style.ChildBorderSize = 1.f;
		style.WindowPadding = ImVec2(0.f, 0.f);
		style.ItemSpacing = ImVec2(0.f, 0.f);
		//style.FramePadding = ImVec2(0.f, 0.f);


		style.TabRounding = 4.f;
		style.FrameRounding = 1.f;
		//style.ScrollbarRounding = 9.f;



		auto& color = style.Colors;
		color[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f); //타이틀
		color[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
		color[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
		color[ImGuiCol_ResizeGrip] = ImVec4(0.f, 0.f, 0.f, 0.f); //리사이즈 그립
		//color[ImGuiCol_ResizeGripHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
		//color[ImGuiCol_ResizeGripActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
		color[ImGuiCol_TabActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.f); //탭
		color[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
		color[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.12f, 1.f);
		color[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 1.f);
		color[ImGuiCol_Border] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);// 테두리
		color[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.f); //메뉴
		color[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.f); //본문
		color[ImGuiCol_Separator] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);// 본문 줄긋기

		color[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.f);

		color[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
		color[ImGuiCol_TableBorderStrong] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
		color[ImGuiCol_TableBorderLight] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

		color[ImGuiCol_Header] = ImVec4(1.f, 1.f, 1.f, 0.03f); // 기본
		color[ImGuiCol_HeaderHovered] = ImVec4(1, 1, 1, 0.06f); // 호버
		color[ImGuiCol_HeaderActive] = ImVec4(1, 1, 1, 0.12f); // 눌림(펼침)

		color[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.f);
		color[ImGuiCol_ButtonHovered] = ImVec4(1, 1, 1, 0.06f);
		color[ImGuiCol_ButtonActive] = ImVec4(1, 1, 1, 0.12f);

		_editor.emplace_back(std::move(library::make_unique<menu>()));
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
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

		for (auto& editor : _editor)
			editor->update();
	}
	void editors::render(void) noexcept {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
