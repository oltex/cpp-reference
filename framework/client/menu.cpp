#include "menu.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"
#include "library/tinygltf/tiny_gltf.h"
#include "graphic.h"
#include "window.h"

#include "resource.h"
#include "object.h"
#include "mesh.h"
#include "transform.h"
#include "texture.h"

#include <windows.h>
#include <shobjidl.h>
#include <cassert>

namespace framework {
	void menu::update(void) noexcept {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Import")) {

					IFileOpenDialog* file;
					if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&file)))) {
						COMDLG_FILTERSPEC filter[]{
							{ L"All Files (*.*)", L"*.*" }
						};
						file->SetFileTypes(sizeof(filter) / sizeof(filter[0]), filter);
						file->SetFileTypeIndex(1);
						file->SetDefaultExtension(L"gltf");

						if (SUCCEEDED(file->Show(window::instance().data()))) {
							IShellItem* item;
							if (SUCCEEDED(file->GetResult(&item))) {
								PWSTR path;
								if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
									CoTaskMemFree(path);

									open_gltf("HatKid.glb");
								}
								item->Release();
							}
						}
						file->Release();
					}


				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	void menu::open_gltf(char const* const path) noexcept {
		auto& resources = resources::instance();
		auto& objects = objects::instance();
		auto object = objects.allocate_object();
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;

		std::string _path(path);
		loader.LoadBinaryFromFile(&model, nullptr, nullptr, path);
		for (auto& mesh : model.meshes) {
			resources.create<framework::mesh>(mesh.name.c_str(), model, mesh);
		}
		library::vector<library::intrusive::share_pointer<framework::transform, 0>> bones;
		for (auto& node : model.nodes) {
			auto bone = object->add_component<framework::transform>(node.name.c_str(), node.translation, node.rotation, node.scale);
			bones.emplace_back(bone);
		}
		//for (auto& bone : bones) {
		//}
	}
}