#include "client.h"
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

		_scenes.create_scene("");
		_scenes.update_system();


		//auto material = _resources.create_resource<framework::material>("material");
		//material->add_texture("texture");

		//resource
		auto texture = _resources.create_resource<framework::texture>("texture", L"images.png");
		library::vector<d3d11::input_element_descript> descript{
			d3d11::input_element_descript{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			d3d11::input_element_descript{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		auto shader = _resources.create_resource<framework::shader>("shader", L"VertexShader.hlsl", "main", "vs_5_0", L"PixelShader.hlsl", "main", "ps_5_0", descript);

		//object
		auto camera = _scenes.create_object();
		auto camera_component = camera->add_component<framework::camera>("camera", 60.f, 1280.f, 720.f, 0.2f, 500.f);
		auto camera_transform = camera->add_component<framework::transform>("transform");
		auto object = _scenes.create_object();
		auto object_tramsform = object->add_component<framework::transform>("transform");
		auto object_renderer = object->add_component<framework::renderer>("renderer");

		auto mesh = _resources.find_resource<framework::mesh>("sprite");
		object_renderer->add_draw_item(object_tramsform, mesh, texture);

		//system
		auto pipeline = _scenes.create_render_system<framework::pipeline>();
		library::vector<library::intrusive::share_pointer<component, 0>> com{ camera_component, camera_transform };
		pipeline->add_component("camera", com);
		com.clear();
		com.emplace_back(object_renderer);
		pipeline->add_component("object", com);
		//pipeline->set_camera(camera_component, camera_transform);
	}
	client::~client(void) noexcept {
	}

	void client::execute(void) noexcept {
		for (;;) {
			if (_window.check_exit())
				return;
			_timer.update_frame();
			_input.update_state();
			_audio.update_sound();
			_scenes.update_system();
			_editors.update();

			_scenes.render_system();
			_editors.render();
			_graphic.render_start();
			_timer.sleep_frame();
		}
	}
}