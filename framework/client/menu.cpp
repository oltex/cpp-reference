#include "menu.h"

namespace framework {
	void menu::update(void) noexcept {

		if (ImGui::BeginMainMenuBar()) {
			ImGui::EndMainMenuBar();
		}


		//const char* items[] = { "Static", "Loading", "Logo", "Lobby", "GamePlay" };
		//static int item_current_idx = 0;
		//const char* combo_preview_value = items[item_current_idx];
		//if (ImGui::BeginCombo("Scene", combo_preview_value)) {
		//	for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
		//		const bool is_selected = (item_current_idx == n);
		//		if (ImGui::Selectable(items[n], is_selected)) {
		//			item_current_idx = n;
		//			dynamic_cast<CInspector*>(pGame_Instance->Get_Edit(TEXT("Inspector")))->Set_Object(nullptr);
		//			dynamic_cast<CNavMesh*>(pGame_Instance->Get_Edit(TEXT("NavMesh")))->Set_Navi(nullptr);
		//			pGame_Instance->Clear_Scene();
		//			pGame_Instance->Change_Scene(static_cast<_ubyte>(EScene::LOADING), CLoading::Create(m_pDevice, m_pContext, pNetwork, static_cast<EScene>(item_current_idx)));
		//		}
		//	}
		//	ImGui::EndCombo();
		//}
	}
}
