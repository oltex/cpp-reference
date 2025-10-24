#include "scene.h"
#include "client.h"

#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

namespace framework {
	void scenes::update_system(void) noexcept {
		if (nullptr != _next_scene)
			_current_scene = std::move(_next_scene);
		if (nullptr != _current_scene)
			for (auto& iter : _current_scene->_system)
				iter.update();
	}
	void scenes::render_system(void) noexcept {
		if (nullptr != _current_scene) {
			for (auto& iter : _current_scene->_render_system) {
				iter.update();
			}
		}
	}
	void scenes::create_scene(library::string const& path) noexcept {
		_next_scene.set(new scene);
	}
	auto scenes::create_object(void) noexcept -> library::intrusive::share_pointer<object, 0> {
		auto pointer = objects::instance().allocate_object();
		_current_scene->_object.push_back(pointer);
		return pointer;
	}
	auto scenes::clone_object(library::intrusive::share_pointer<object, 0>& origin) noexcept -> library::intrusive::share_pointer<object, 0> {
		auto pointer = objects::instance().allocate_object(*origin);
		_current_scene->_object.push_back(pointer);
		return pointer;
	}
	void scenes::destory_object(library::intrusive::share_pointer<object, 0>& pointer) noexcept {
		library::intrusive::pointer_list<object, 0, 0>::erase(pointer);
	}

	void scenes::edit(void) noexcept {
		if (ImGui::Begin("Scene", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::EndMenuBar();
			}
		}
		ImGui::End();
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