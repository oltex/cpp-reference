#include "resource.h"
#include "sound.h"
#include "mesh.h"
#include <filesystem>
#include <fstream>

#include "window.h"
#include "texture.h"
#include "sound.h"
#include "library/system/file.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

namespace framework {
	resource::resource(library::string_view name) noexcept
		: _name(name), _guid(library::create_guid()) {
	}
	resource::resource(nlohmann::json const& json) noexcept {
		_name = json["name"].get_ref<std::string const&>().c_str();
		_guid = library::string(json["guid"].get_ref<std::string const&>().c_str());
	}
	auto resource::name(void) noexcept -> library::string& {
		return _name;
	}
	auto resource::guid(void) noexcept -> library::guid& {
		return _guid;
	}
	void resource::save(nlohmann::json& json) noexcept {
		json["name"] = _name.data();
		json["guid"] = _guid.string().data();
	}

	resources::resources(void) noexcept {
		//{
		//	struct vertex_face {
		//		dmath::float3 _position;
		//		dmath::float2 _texcoord;
		//	};
		//	library::vector<vertex_face> vertex{
		//		{ {-0.5f,  0.5f, 0.f}, {0.f, 0.f} },
		//		{ { 0.5f,  0.5f, 0.f}, {1.f, 0.f} },
		//		{ {-0.5f, -0.5f, 0.f}, {1.f, 1.f} },
		//		{ { 0.5f, -0.5f, 0.f}, {0.f, 1.f} },
		//	};
		//	library::vector<unsigned short> index{
		//		0, 1, 2, 1, 3, 2
		//	};
		//	create_resource<framework::mesh>("sprite_mesh", vertex, index);
		//}
		//{
		//	struct vertex_face {
		//		dmath::float3 _position;
		//		dmath::float3 _texcoord;
		//	};
		//	library::vector<vertex_face> vertex{
		//		{ {-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f} },
		//		{ { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f} },
		//		{ {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f} },
		//		{ { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f} },
		//		{ {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f} },
		//		{ { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f} },
		//		{ {-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f} },
		//		{ { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f} }
		//	};
		//	library::vector<unsigned short> index{
		//		0, 1, 2,  1, 3, 2,
		//		0, 4, 1,  4, 5, 1,
		//		1, 5, 3,  5, 7, 3,
		//		3, 7, 2,  7, 6, 2,
		//		2, 6, 0,  6, 4, 0,
		//		5, 4, 7,  4, 6, 7
		//	};
		//	create_resource<framework::mesh>("cube_mesh", vertex, index);
		//}
	}
	resources::~resources(void) noexcept {
		for (auto& iter : _item) {
			auto pointer = &(*iter);
			_pool.find(pointer->type())->_second->deallocate(pointer);
		}
	}
	void resources::destory(library::rcu_pointer<resource> pointer) noexcept {
		pointer.invalid([&](resource* pointer) {
			auto& result = _pool.find(reinterpret_cast<resource*>(pointer)->type())->_second;
			result->deallocate(reinterpret_cast<resource*>(pointer));
			});
	}
	void resources::save(void) noexcept {
		std::filesystem::create_directories("resource");
		nlohmann::json json;

		for (auto& iter : _item) {
			iter->type();
			auto& array = json[iter->type()];
			if (!array.is_array())
				array = nlohmann::json::array();
			nlohmann::json segment;
			iter->save(segment); 
			array.push_back(std::move(segment));
		}

		const std::filesystem::path path = std::filesystem::path("resource") / "textures.json";
		std::ofstream ofs(path, std::ios::binary | std::ios::trunc);
		ofs << json.dump(2);
		ofs.flush();
	}
	void resources::load(void) noexcept {
		const std::filesystem::path path = std::filesystem::path("resource") / "textures.json";
		std::ifstream ifs(path);
		if (!ifs.is_open())
			return;
		nlohmann::json json;
		ifs >> json;

		for (auto& [key, arr] : json.items()) {
			auto& pool = _pool.find(key.c_str())->_second;
			for (auto& meta : arr) {
				auto pointer = pool->allocate(meta);
				_guid.emplace(pointer->guid(), pointer);
				_name.emplace(pointer->name(), pointer);
				_item.emplace_back(pointer);
			}
		}
	}

	void resources::update(void) noexcept {
		if (ImGui::Begin("Resource", 0, ImGuiWindowFlags_MenuBar)) {
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

				//if (auto sort_specs = ImGui::TableGetSortSpecs(); true == sort_specs->SpecsDirty) {
				//	std::sort(_item.begin(), _item.end(), [&sort_specs](auto& lhs, auto& rhs) noexcept {
				//		for (int n = 0; n < sort_specs->SpecsCount; n++) {
				//			auto sort_spec = &sort_specs->Specs[n];

				//			int delta = 0;
				//			switch (sort_spec->ColumnUserID) {
				//			case column::name:
				//				delta = std::strcmp(lhs->name().data(), rhs->name().data());
				//				break;
				//			case column::type:
				//				delta = std::strcmp(lhs->type(), rhs->type());
				//				break;
				//			}
				//			if (delta > 0)
				//				return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? true : false;
				//			if (delta < 0)
				//				return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? false : true;
				//		}
				//		return false;
				//		});
				//	sort_specs->SpecsDirty = false;
				//}

				ImGuiListClipper clipper;
				int erase = -1;
				clipper.Begin(static_cast<int>(_item.size()));
				while (clipper.Step())
					for (auto index = clipper.DisplayStart; index < clipper.DisplayEnd; index++) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Selectable(_item[index]->name().data(), false, ImGuiSelectableFlags_SpanAllColumns);

						if (ImGui::BeginPopupContextItem()) {
							if (ImGui::MenuItem("Delete"))
								erase = index;
							ImGui::EndPopup();
						}
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
							//ImGui::SetDragDropPayload("ASSET_GUID", &_item[index]._guid, sizeof(_item[index]._guid));
							ImGui::TextUnformatted(_item[index]->name().data());
							ImGui::EndDragDropSource();
						}

						ImGui::TableNextColumn();
						ImGui::Text("%s", _item[index]->type());
					}
				if (-1 != erase) {
					auto& item = _item[erase];
					_item.erase(_item.begin() + erase);
					destory(item);
				}
				ImGui::EndTable();
			}

		}
		ImGui::End();
	}
	void resources::import_file(void) noexcept {
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
					auto pointer = create<framework::texture>(path.stem().string().c_str(), path.string().c_str());
					_item.emplace_back(pointer);
				}
				else if (".wav" == path.extension().string()) {
					auto pointer = create<framework::sound>(path.stem().string().c_str(), path.string().c_str());
					_item.emplace_back(pointer);
				}
			}
		}
		ImGui::PopStyleColor(3);
	}
	void resources::search(void) noexcept {
		constexpr float width = 220.f;
		ImGui::Dummy(ImVec2(library::maximum(0.0f, ImGui::GetContentRegionAvail().x - width), 0));
		ImGui::SetNextItemWidth(width);
		static char query[128] = {};
		ImGui::InputTextWithHint("", "Search", query, sizeof(query));
	}
}
