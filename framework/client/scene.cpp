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
		if (nullptr != _next_scene)
			_current_scene = std::move(_next_scene);
		if (nullptr != _current_scene)
			for (auto& iter : _current_scene->_system)
				iter.update();

		if (ImGui::Begin("Scene", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::Button("Create Object")) {
					create_object();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Separator();

			if (ImGui::BeginTable("layout", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn("Hierarchy", ImGuiTableColumnFlags_WidthStretch, 3.0f);
				ImGui::TableSetupColumn("Inspector", ImGuiTableColumnFlags_WidthStretch, 5.0f);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				edit();
				ImGui::TableSetColumnIndex(1);
				ImGui::EndTable();
			}
		}
		ImGui::End();
	}
	void scenes::render(void) noexcept {
		if (nullptr != _current_scene) {
			for (auto& iter : _current_scene->_render_system) {
				iter.update();
			}
		}
	}

	void scenes::edit(void) noexcept {
		ImGui::BeginChild("##frame", ImVec2(0, 0), 0, ImGuiWindowFlags_NoScrollbar);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
		if (ImGui::TreeNodeEx("Object", ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth)) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 0.03f));
			ImGui::Indent(5);

			static ImGuiSelectionBasicStorage selection;
			auto ms_io = ImGui::BeginMultiSelect(ImGuiMultiSelectFlags_BoxSelect1d, selection.Size, _current_scene->_object.size());
			selection.ApplyRequests(ms_io);

			ImGuiListClipper clipper;
			clipper.Begin(_current_scene->_object.size());
			if (ms_io->RangeSrcItem != -1) {
				clipper.IncludeItemByIndex(static_cast<int>(ms_io->RangeSrcItem));
				inspector::instance().set(_current_scene->_object[ms_io->RangeSrcItem]);
			}
			while (clipper.Step()) {
				for (auto index = clipper.DisplayStart; index < clipper.DisplayEnd; index++) {
					ImGui::PushID(index);
					ImGui::SetNextItemSelectionUserData(index);
					bool selected = selection.Contains(index);
					ImGui::Selectable(_current_scene->_object[index]->name().data(), selected);
					ImGui::PopID();
				}
			}

			selection.ApplyRequests(ImGui::EndMultiSelect());

			ImGui::Unindent(5);
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("System", ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth)) {
			ImGui::TreePop();
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}

	void scenes::create_scene(library::string const& path) noexcept {
		_current_scene = library::make_unique<scene>();
	}
	auto scenes::create_object(void) noexcept ->  library::rcu_pointer<object> {
		auto pointer = objects::instance().allocate();
		_current_scene->_object.emplace_back(pointer);
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