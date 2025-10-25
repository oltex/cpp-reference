#include "scene.h"
#include "client.h"

#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

#include "graphic.h"

namespace framework {
	scenes::scenes(void) noexcept
		: _screen(d3d11::texture_2d_descript(1424, 720, 1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, 0), nullptr, nullptr, nullptr, nullptr),
		_depth(d3d11::texture_2d_descript(1424, 720, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL, 0, 0), nullptr, nullptr, nullptr, nullptr) {
	}
	void scenes::update(void) noexcept {
		if (nullptr != _next_scene)
			_current_scene = std::move(_next_scene);
		if (nullptr != _current_scene)
			for (auto& iter : _current_scene->_system)
				iter.update();


		if (ImGui::Begin("Scene", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::EndMenuBar();
			}

			ImGui::Image((ImTextureID)(_screen._srv.data()), ImVec2(1424, 720), ImVec2(0, 0), ImVec2(1, 1));
		}
		ImGui::End();
	}
	void scenes::render_system(void) noexcept {
		if (nullptr != _current_scene) {
			for (auto& iter : _current_scene->_render_system) {
				iter.update();
			}
		}
	}
	void scenes::create_scene(library::string const& path) noexcept {
		_current_scene = library::make_unique<scene>();
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