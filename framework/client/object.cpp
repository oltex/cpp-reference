#include "object.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

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
		if (ImGui::TreeNodeEx("Object",
			ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf)) {

			if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {

			}
			if (ImGui::Button("Add Component")) {
				ImGui::TreePop();
			}
		}
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