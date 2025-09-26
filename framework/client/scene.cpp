#include "scene.h"
#include "client.h"

namespace framework {
	void scenes::update_scene(void) noexcept {
		if (nullptr != _next_scene)
			_current_scene = std::move(_next_scene);
		if (nullptr != _current_scene) {
			for (auto& iter : _current_scene->_system) {
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