#include "client.h"
#include "sound.h"

#include "behave.h"
#include "transform.h"
#include "camera.h"

typedef struct SVertex {
	dmath::float3 _position;
}VTX;
typedef struct SIndex_Face16 {
	unsigned short _1, _2, _3;
}IDXFACE16;

namespace framework {
	client::client(void) noexcept
		: _window(), _graphic(graphic::construct(_window)), _audio(audio::instance()), _input(input::instance())
		, _scenes(scenes::instance()), _objects(objects::instance()), _components(components::instance()), _resources(resources::instance()) {

		_scenes.create_scene("");
		_scenes.update_scene();
		auto object = _scenes.create_object();
		auto camera = object->add_component<framework::camera>("camera");
		auto transform = object->add_component<framework::transform>("transform");

		library::vector<SVertex> vertex;
		vertex.resize(4);
		vertex[0]._position = dmath::float3(-0.5f, 0.5f, 0.f);
		vertex[1]._position = dmath::float3(0.5f, 0.5f, 0.f);
		vertex[2]._position = dmath::float3(0.5f, -0.5f, 0.f);
		vertex[3]._position = dmath::float3( -0.5f, -0.5f, 0.f);

		library::vector<IDXFACE16> index;
		index.resize(2);
		index[0] = IDXFACE16(0, 1, 2);
		index[1] = IDXFACE16(0, 2, 3);
		_resources.create_mesh("triangle", vertex, index);
	}
	client::~client(void) noexcept {
		_graphic.destruct();
	}

	void client::execute(void) noexcept {
		for (;;) {
			if (_window.is_exit())
				return;
			_timer.update_frame();
			_input.update_state();
			_audio.update_sound();

			_scenes.update_scene();

			_graphic.render_ready();
			_graphic.render_start();
			_timer.sleep_frame();
		}
	}
}