#include "client.h"
#include "sound.h"

#include "behave.h"
#include "transform.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"

struct vertex_face {
	dmath::float3 _position;
};
struct index_face16 {
	unsigned short _1, _2, _3;
};
D3D11_INPUT_ELEMENT_DESC desc_face[1] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

namespace framework {
	client::client(void) noexcept
		: _window(), _graphic(graphic::construct(_window)), _audio(audio::instance()), _input(input::instance())
		, _scenes(scenes::instance()), _objects(objects::instance()), _components(components::instance()), _resources(resources::instance()) {

		_scenes.create_scene("");
		_scenes.update_scene();
		auto object = _scenes.create_object();
		auto camera = object->add_component<framework::camera>("camera", 60.f, 1280.f, 720.f, 0.2f, 500.f);
		auto transform = object->add_component<framework::transform>("transform");

		library::vector<vertex_face> vertex;
		vertex.resize(4);
		vertex[0]._position = dmath::float3(-0.5f, 0.5f, 0.f);
		vertex[1]._position = dmath::float3(0.5f, 0.5f, 0.f);
		vertex[2]._position = dmath::float3(0.5f, -0.5f, 0.f);
		vertex[3]._position = dmath::float3( -0.5f, -0.5f, 0.f);
		library::vector<index_face16> index;
		index.resize(2);
		index[0] = index_face16(0, 1, 2);
		index[1] = index_face16(0, 2, 3);
		_resources.create_resource<mesh>("triangle", vertex, index);


		_resources.create_resource<mesh>("triangle", vertex, index);

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