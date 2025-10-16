#include "inspector.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

namespace framework {
    void inspector::update(void) noexcept {
        if (ImGui::Begin("inspector",0, ImGuiWindowFlags_MenuBar)) {
            if (ImGui::BeginMenuBar()) {

                ImGui::EndMenuBar();
            }
            ImGui::Separator();
        }
        ImGui::End();
    }
}
