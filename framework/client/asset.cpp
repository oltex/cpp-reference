#include "asset.h"
#include "window.h"
#include "texture.h"
#include "library/system/file.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

#include <windows.h>
#include <shobjidl.h>
#include <filesystem>
#include <cwctype>
#include <algorithm> 

namespace framework {
	void asset::update(void) noexcept {
		if (ImGui::Begin("Asset", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				import_file();
				search();
				ImGui::EndMenuBar();
			}
			ImGui::Separator();

			ImGui::Columns(2);
			ImGui::SameLine();

			if (ImGui::BeginTable("File", 2, ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody, ImVec2(0.f, 0.f))) {
				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0, column::name);
				ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 0, column::type);
				ImGui::TableHeadersRow();

				if (auto sort_specs = ImGui::TableGetSortSpecs(); true == sort_specs->SpecsDirty) {
					std::sort(_item.begin(), _item.end(), [&sort_specs](auto& lhs, auto& rhs) noexcept {
						for (int n = 0; n < sort_specs->SpecsCount; n++) {
							auto sort_spec = &sort_specs->Specs[n];

							int delta = 0;
							switch (sort_spec->ColumnUserID) {
							case column::name:
								delta = std::strcmp(lhs._name.c_str(), rhs._name.c_str());
								break;
							case column::type:
								delta = std::strcmp(lhs._type.c_str(), rhs._type.c_str());
								break;
							}
							if (delta > 0)
								return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? true : false;
							if (delta < 0)
								return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? false : true;
						}
						return false;
						});
					sort_specs->SpecsDirty = false;
				}

				ImGuiListClipper clipper;
				int erase = -1;
				clipper.Begin(static_cast<int>(_item.size()));
				while (clipper.Step())
					for (auto index = clipper.DisplayStart; index < clipper.DisplayEnd; index++) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Selectable(_item[index]._name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

						if (ImGui::BeginPopupContextItem()) {
							if (ImGui::MenuItem("Delete"))
								erase = index;
							ImGui::EndPopup();
						}
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
							//ImGui::SetDragDropPayload("ASSET_GUID", &_item[index]._guid, sizeof(_item[index]._guid));
							ImGui::TextUnformatted(_item[index]._name.c_str());
							ImGui::EndDragDropSource();
						}

						ImGui::TableNextColumn();
						ImGui::Text("%s", _item[index]._type.c_str());
					}
				if (-1 != erase) {
					auto& item = _item[erase];
					_item.erase(_item.begin() + erase);
					resources::instance().deallocate_resource(item._pointer);
				}
				ImGui::EndTable();
			}

		}
		ImGui::End();
	}

	void asset::import_file(void) noexcept {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.06f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.12f));
		if (ImGui::Button("Import")) {
			library::file_open_dialog dialog;
			COMDLG_FILTERSPEC filter[]{ { L"All Files (*.*)", L"*.*" } };
			dialog.set_file_type(filter);
			dialog.set_file_type_index(1);
			if (dialog.show(window::instance().data())) {
				auto item = dialog.get_result();
				auto name = item.get_display_name(SIGDN_FILESYSPATH);
				auto path = std::filesystem::path(name);
				library::component_task_memory_free(name);

				if (".png" == path.extension().string()) {
					auto texture = resources::instance().create_resource<framework::texture>(path.stem().string().c_str(), path.wstring().c_str());
					_item.push_back(asset::item{ texture, path.stem().string(), "texture" });
				}
			}
		}
		ImGui::PopStyleColor(3);
	}
	void asset::search(void) noexcept {
		constexpr float width = 220.f;
		ImGui::Dummy(ImVec2(library::maximum(0.0f, ImGui::GetContentRegionAvail().x - width), 0));
		ImGui::SetNextItemWidth(width);
		static char query[128] = {};
		ImGui::InputTextWithHint("", "Search", query, sizeof(query));
	}
}
