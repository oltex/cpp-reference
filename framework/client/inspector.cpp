#include "inspector.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

namespace framework {
	void inspector::update(void) noexcept {
		if (ImGui::Begin("inspector", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::EndMenuBar();
			}
			ImGui::Separator();

			if (auto pointer = std::get_if<library::rcu_pointer<object>>(&_pointer)) {
				if (auto& object = *pointer; !object)
					_pointer.emplace<std::monostate>();
				else
					object->edit();
			}
		}
		ImGui::End();
	}
	void inspector::set(library::rcu_pointer<object> pointer) noexcept {
		_pointer = pointer;
	}
}
