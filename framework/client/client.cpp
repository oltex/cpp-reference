#include "client.h"
#include "sound.h"
namespace framework {
	client::client(void) noexcept
		: _window(), _graphic(graphic::construct(_window)), _audio(audio::instance()) {
		//auto camera = create_object();
		//camera->add_component("transform", &create_component<framework::transform>());
		//camera->add_component("camera", &create_component<framework::camera>());
		//regist_object("camera", camera);

		//create_scene("hello");

		auto my_sound = sound("applause.mp3", FMOD_DEFAULT);
		auto my_sound2(my_sound);
	}

	client::~client(void) noexcept {
		_graphic.destruct();
	}

	void client::execute(void) noexcept {
		for (;;) {
			if (_window.is_exit())
				return;
			_timer.update_frame();
			//_scene_manager.update();
			_input.update_state();
			_audio.update_sound();

			_graphic.render_ready();
			_graphic.render_start();
			_timer.sleep_frame();
		}
	}
	//auto client::create_scene(library::string const& path) noexcept {
		//_scene_manager.create_scene(path);
	//}
}