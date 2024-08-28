#pragma once
#include "design-pattern/singleton.h"
#include "window/window.h"
#include "graphic.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

namespace engine {
	class editor final : public design_pattern::singleton<editor, design_pattern::member_static<editor>> {
		friend class design_pattern::singleton<editor, design_pattern::member_static<editor>>;
	public:
		inline static auto constructor(window::window& window, graphic& graphic) noexcept -> editor& {
			_instance = new editor(window, graphic);
			return *_instance;
		}
	private:
		inline explicit editor(window::window& window, graphic& graphic) noexcept {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui::StyleColorsDark();
			//ImGuiStyle& style = ImGui::GetStyle();
			//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			//	style.WindowRounding = 0.0f;
			//	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			//}

			ImGui_ImplWin32_Init(window.data());
			ImGui_ImplDX11_Init(&graphic.get_device(), &graphic.get_context());
		};
		inline explicit editor(editor const& rhs) noexcept = delete;
		inline auto operator=(editor const& rhs) noexcept -> editor & = delete;
		inline explicit editor(editor&& rhs) noexcept = delete;
		inline auto operator=(editor&& rhs) noexcept -> editor & = delete;
		inline ~editor(void) noexcept {
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		};
	public:
		inline void update(void) noexcept {
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			
			{ // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::End();
			}
		}
		inline void render(void) noexcept {
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	};
}