#include "asset.h"
#include "window.h"
#include "resource.h"
#include "texture.h"
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
import();

				search();
				ImGui::EndMenuBar();
			}
			ImGui::Separator();


			ImGui::Columns(2);
			ImGui::SameLine();

			if (ImGui::BeginTable("3ways", 2, ImGuiTableFlags_Sortable | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody, ImVec2(0.f, 0.f))) {
				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
				ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableHeadersRow();

				static ImGuiTreeNodeFlags tree_node_flags_base = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;
				struct mnode {
					const char* Name;
					const char* Type;
					int ChildIdx;
					int ChildCount;
					static void DisplayNode(const mnode* node, const mnode* all_nodes) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						const bool is_folder = (node->ChildCount > 0);

						ImGuiTreeNodeFlags node_flags = tree_node_flags_base;
						if (node != &all_nodes[0])
							node_flags &= ~ImGuiTreeNodeFlags_LabelSpanAllColumns; // Only demonstrate this on the root node.

						if (is_folder) {
							bool open = ImGui::TreeNodeEx(node->Name, node_flags);
							if ((node_flags & ImGuiTreeNodeFlags_LabelSpanAllColumns) == 0) {
								ImGui::TableNextColumn();
								ImGui::TextUnformatted(node->Type);
							}
							if (open) {
								for (int child_n = 0; child_n < node->ChildCount; child_n++)
									DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
								ImGui::TreePop();
							}
						}
						else {
							ImGui::TreeNodeEx(node->Name, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
							ImGui::TableNextColumn();
							ImGui::TextUnformatted(node->Type);
						}
					}
				};
				static const mnode nodes[] =
				{
					{ "Root with Long Name",          "Folder",        1, 3    }, // 0
					{ "Music",                        "Folder",        4, 2    }, // 1
					{ "Textures",                     "Folder",        6, 3    }, // 2
					{ "desktop.ini",                  "System file",  -1,-1    }, // 3
					{ "File1_a.wav",                  "Audio file",   -1,-1    }, // 4
					{ "File1_b.wav",                  "Audio file",   -1,-1    }, // 5
					{ "Image001.png",                 "Image file",   -1,-1    }, // 6
					{ "Copy of Image001.png",         "Image file",   -1,-1    }, // 7
					{ "Copy of Image001 (Final2).png","Image file",   -1,-1    }, // 8
				};
				mnode::DisplayNode(&nodes[0], nodes);
				ImGui::EndTable();
			}

		}
		ImGui::End();
	}

	void asset::import(void) noexcept {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.06f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.12f));
		if (ImGui::Button("Import")) {
			IFileOpenDialog* file;
			if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&file)))) {
				COMDLG_FILTERSPEC filter[]{
					{ L"All Files (*.*)", L"*.*" }
				};
				file->SetFileTypes(sizeof(filter) / sizeof(filter[0]), filter);
				file->SetFileTypeIndex(1);

				if (SUCCEEDED(file->Show(window::instance().data()))) {
					IShellItem* item;
					if (SUCCEEDED(file->GetResult(&item))) {
						PWSTR _path;
						if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &_path))) {
							auto path = std::filesystem::path(_path);

							if (".png" == path.extension().string())
								resources::instance().create_resource<framework::texture>(path.filename().string().c_str(), path.wstring().c_str());
							CoTaskMemFree(_path);
						}
						item->Release();
					}
				}
				file->Release();
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
