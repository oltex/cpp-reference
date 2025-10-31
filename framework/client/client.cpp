#include "client.h"
#include "library/container/read_copy_update.h"

#include "sound.h"

#include "pipeline.h"
#include "behave.h"

#include "transform.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "renderer.h"
#include "texture.h"
#include "material.h"
#include "camera_move.h"
#include <typeinfo>
#include <iostream>

#include "inspector.h"

namespace framework {
	client::client(void) noexcept
		: _window(window::instance()),
		_graphic(graphic::instance()),
		_editors(editors::instance()),
		_audio(audio::instance()),
		_input(input::instance()),
		_scenes(scenes::instance()),
		_objects(objects::instance()),
		_components(components::instance()),
		_resources(resources::instance()) {

		_resources.load();

		//resource
		//library::vector<d3d11::input_element_descript> descript{
		//	d3d11::input_element_descript{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	d3d11::input_element_descript{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		//};
		//auto shader = _resources.create_resource<framework::shader>(L"sprite_vertex.hlsl", L"sprite_pixel.hlsl", descript);
		//auto material = _resources.create_resource<framework::material>();
		//material->set_shader(shader);
		//auto mesh = _resources.find_resource<framework::mesh>();

		//object
		//auto camera = _scenes.create_object();
		//auto camera_transform = camera->add_component<framework::transform>("transform");
		//auto camera_component = camera->add_component<framework::camera>("camera", 60.f, 1280.f, 720.f, 0.2f, 500.f);
		//auto camera_move = camera->add_component<framework::camera_move>("camera_move");

		//auto object = _scenes.create_object();
		//auto object_tramsform = object->add_component<framework::transform>("transform");
		//auto object_renderer = object->add_component<framework::renderer>("renderer");
		//object_renderer->set_transform(0, object_tramsform);
		//object_renderer->set_mesh(0, mesh);
		//object_renderer->add_material(0, material);

		//system
		//auto behave = _scenes.create_render_system<framework::behave>();
		//auto pipeline = _scenes.create_render_system<framework::pipeline>();
		//library::vector<library::intrusive::share_pointer<component, 0>> com{ camera_component, camera_transform };
		//pipeline->add_component("camera", com);
		////com.clear();
		////com.emplace_back(object_renderer);
		////pipeline->add_component("object", com);
		//com.clear();
		//com.emplace_back(camera_move);
		//behave->add_component("behaviour", com);

	}
	client::~client(void) noexcept {

		_resources.save();
	}

	void client::execute(void) noexcept {
		for (;;) {
			auto& rcu = library::read_copy_update::instance();
			rcu.lock();
			if (_window.check_exit())
				return;
			_timer.update_frame();
			_input.update_state();
			_audio.update_sound();

			_editors.update();
			_scenes.update();
			_graphic.update();
			_resources.edit();
			_scenes.edit();
			inspector::instance().update();

			_graphic.render();
			_scenes.render();
			_editors.render();
			_graphic.present();
			_timer.sleep();
			rcu.unlock();
		}
	}
}