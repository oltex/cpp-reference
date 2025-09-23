#include "client.h"

namespace framework {
	client::client(void) noexcept
		: _window(), _graphic(_window) {
		auto camera = create_object();
		camera->add_component("transform", &create_component<framework::transform>());
		camera->add_component("camera", &create_component<framework::camera>());
		regist_object("camera", camera);

		//create_scene("hello");
	}

	void client::execute(void) noexcept {
		for (;;) {
			if (_window.is_exit())
				return;
			_frame.update();
			_scene_manager.update();
			_graphic.render();
			_frame.sleep();
		}
	}
	auto client::create_scene(library::string const& path) noexcept {
		_scene_manager.create_scene(path);
	}
}