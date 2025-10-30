#include "object.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

#include "transform.h"

namespace framework {
	object::object(library::string_view name) noexcept
		: _guid(library::create_guid()), _name(name) {
	}
	object::object(object const& rhs) noexcept {
	}

	auto object::name(void) noexcept -> library::string& {
		return _name;
	}

	void object::edit(void) noexcept {
		//if (ImGui::TreeNodeEx("Object",
		//	ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf)) {

			static bool mode = false;

			if (false == mode) {
				for (auto& iter : _component) {
					iter._second->edit();
				}

				if (ImGui::Button("Add Component")) {
					mode = true;
				}
			}
			else {
				constexpr float width = 220.f;
				ImGui::Dummy(ImVec2(library::maximum(0.0f, ImGui::GetContentRegionAvail().x - width), 0));
				ImGui::SetNextItemWidth(width);
				static char query[128] = {};
				ImGui::InputTextWithHint("##", "Search", query, sizeof(query));
				if (ImGui::Button("X##AddCompClose", ImVec2(10, 10))) {  // ID 충돌 방지용 ##접미사
					mode = false;                                      // 선택 모드 종료
				}
				if (ImGui::Button("Transform")) {
					add_component<transform>("transform");
				}
					
			}
		//}
	}


	void objects::deallocate(library::rcu_pointer<object> pointer) noexcept {
		pointer.invalid([&](object* pointer) {
			_pool.deallocate(pointer);
			});
	}
	//void objects::regist_prototype(library::string const& name, library::intrusive::share_pointer<object, 0>& object) noexcept {
	//	//auto clone = allocate_object(*object);
	//	//_prototype.emplace(name, clone);
	//}
	//auto objects::find_prototype(library::string const& name) noexcept -> library::intrusive::share_pointer<object, 0> {
	//	//auto result = _prototype.find(name);
	//	//return result->_second;
	//}
}