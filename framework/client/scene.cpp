#include "scene.h"
#include "client.h"

#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

#include "graphic.h"
#include "inspector.h"

namespace framework {

	scene::~scene(void) noexcept {
	}

	scenes::scenes(void) noexcept {
		//_depth(d3d11::texture_2d_descript(1424, 720, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL, 0, 0), nullptr, nullptr, nullptr, nullptr)
	}
	scenes::~scenes(void) noexcept {
	}
	void scenes::update(void) noexcept {
		//if (nullptr != _next_scene)
		//	_current_scene = std::move(_next_scene);
		//if (nullptr != _current_scene)
		//	for (auto& iter : _current_scene->_system)
		//		iter.update();


	}
	void scenes::render(void) noexcept {
		for (auto& iter : _render_system) {
			iter.update();
		}
	}

	void scenes::edit(void) noexcept {

		if (ImGui::Begin("Scene", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::Button("Save")) {
					resources::instance().create_level();
				}
				if (ImGui::Button("Create Object")) {
					create_object();
				}
				ImGui::EndMenuBar();
			}
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 4.f));
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(8.f, 0.f));
			ImGui::BeginChild("##frame", ImVec2(0, 0), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar);
			if (ImGui::TreeNodeEx("Object", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
				static ImGuiSelectionBasicStorage selection;
				auto ms_io = ImGui::BeginMultiSelect(ImGuiMultiSelectFlags_BoxSelect1d, selection.Size, _object.size());
				selection.ApplyRequests(ms_io);

				ImGuiListClipper clipper;
				clipper.Begin(_object.size());
				if (ms_io->RangeSrcItem != -1) {
					clipper.IncludeItemByIndex(static_cast<int>(ms_io->RangeSrcItem));
				}
				while (clipper.Step()) {
					for (auto index = clipper.DisplayStart; index < clipper.DisplayEnd; index++) {
						ImGui::PushID(index);
						ImGui::SetNextItemSelectionUserData(index);

						static int rename = -1;
						if (rename != index) {
							if (ImGui::Selectable(_object[index]->name().data(), selection.Contains(index))) {
								inspector::instance().set(_object[index]);
							}
							if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							}
							if (ImGui::BeginPopupContextItem()) {
								if (ImGui::MenuItem("Rename"))
									rename = index;
								ImGui::EndPopup();
							}
						}
						else {
							auto& name = _object[index]->name();
							if (ImGui::InputText("##name", name.data(), name.capacity() + 1, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue,
								[](ImGuiInputTextCallbackData* data) noexcept -> int {
									if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
										auto& string = *static_cast<library::string*>(data->UserData);
										string.resize(data->BufTextLen);
										data->Buf = string.data();
									}
									return 0;
								}, &name) || ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsItemDeactivatedAfterEdit()) {
								rename = -1;
							}
						}
						ImGui::PopID();
					}
				}
				selection.ApplyRequests(ImGui::EndMultiSelect());
			}
			if (ImGui::TreeNodeEx("System", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			}
			ImGui::EndChild();
			ImGui::PopStyleVar(2);
		}
		ImGui::End();
	}
	void scenes::open(framework::level& level) noexcept {
	}
	auto scenes::create_object(void) noexcept ->  library::rcu_pointer<object> {
		auto pointer = objects::instance().allocate("Game Object");
		_object.emplace_back(pointer);
		return pointer;
	}
	//auto scenes::clone_object(library::intrusive::share_pointer<object, 0>& origin) noexcept -> library::intrusive::share_pointer<object, 0> {
	//	auto pointer = objects::instance().allocate(*origin);
	//	_current_scene->_object.push_back(pointer);
	//	return pointer;
	//}
	void scenes::destory_object(library::rcu_pointer<object> pointer) noexcept {
	}



	//auto scene::create_object(object_share_ptr& parent) noexcept -> object_share_ptr {
	//	auto& object_mgr = 
	//	auto object = _object_manager.create_object();
	//	return object;
	//}
	//template<typename... argument>
	//inline auto create_object(argument&&... arg) noexcept -> object_share_ptr {
	//	auto object = library::allocate<framework::object>();
	//	library::construct(*object, std::forward<argument>(arg)...);
	//	return object_share_ptr(object);
	//}
	//auto scene::clone_object(library::string const& name, object_share_ptr& parent) noexcept -> object_share_ptr {
	//
	//}
}