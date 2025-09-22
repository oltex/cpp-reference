#include "scene.h"
#include "client.h"

namespace framework {
	scene::scene(void) noexcept {
		auto& client = framework::client::instance();
	}
	void scene::update(void) noexcept {
		for (auto& vector : _system)
			for (auto& system : vector._second)
				system->update();
	}
	auto scene::create_object(object_share_ptr& parent) noexcept -> object_share_ptr {
		auto object = _object_manager.create_object();
		return object;
	}
	template<typename... argument>
	inline auto create_object(argument&&... arg) noexcept -> object_share_ptr {
		auto object = library::allocate<framework::object>();
		library::construct(*object, std::forward<argument>(arg)...);
		return object_share_ptr(object);
	}
	auto scene::clone_object(library::string const& name, object_share_ptr& parent) noexcept -> object_share_ptr {
	
	}
}