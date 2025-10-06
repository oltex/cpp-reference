#include "asset.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

#include "resource.h"

namespace framework {
	void asset::update(void) noexcept {
		if (ImGui::Begin("asset")) {
			auto& resources = resources::instance();
			for (auto& resource : resources._resource) {
				ImGui::Selectable(resource._first.data());
			}
			ImGui::End();
		}
	}
}
