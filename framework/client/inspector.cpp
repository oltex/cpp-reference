#include "inspector.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

namespace framework {
	void inspector::update(void) noexcept {
		if (ImGui::Begin("inspector", 0, ImGuiWindowFlags_MenuBar)){
			if (ImGui::BeginMenuBar()) {
				ImGui::EndMenuBar();
			}
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 4.f));
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(8.f, 0.f));
			ImGui::BeginChild("##frame", ImVec2(0, 0), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar);

			if (auto pointer = std::get_if<library::rcu_pointer<object>>(&_pointer)) {
				if (auto& object = *pointer; !object)
					_pointer.emplace<std::monostate>();
				else
					object->edit();
			}
			ImGui::EndChild();
			ImGui::PopStyleVar(2);

		}
		ImGui::End();
	}
	void inspector::set(library::rcu_pointer<object> pointer) noexcept {
		_pointer = pointer;
	}
}
