#include "client.h"
#include "sound.h"

#include "pipeline.h"
#include "behave.h"

#include "transform.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "renderer.h"


namespace framework {
	client::client(void) noexcept
		: _window(window::instance()),
		_graphic(graphic::instance()),
		_editor(editor::instance()),
		_audio(audio::instance()),
		_input(input::instance()),
		_scenes(scenes::instance()),
		_objects(objects::instance()),
		_components(components::instance()),
		_resources(resources::instance()) {

		_scenes.create_scene("");
		_scenes.update_system();
		{
			//resource
			struct vertex_face {
				dmath::float3 _position;
			};
			library::vector<vertex_face> vertex;
			vertex.resize(4);
			vertex[0]._position = dmath::float3(-0.5f, 0.5f, 0.f);
			vertex[1]._position = dmath::float3(0.5f, 0.5f, 0.f);
			vertex[2]._position = dmath::float3(0.5f, -0.5f, 0.f);
			vertex[3]._position = dmath::float3(-0.5f, -0.5f, 0.f);
			struct index_face16 {
				unsigned short _1, _2, _3;
			};
			library::vector<index_face16> index;
			index.resize(2);
			index[0] = index_face16(0, 1, 2);
			index[1] = index_face16(0, 2, 3);
			_resources.create_resource<mesh>("mesh", vertex, index);

			D3D11_INPUT_ELEMENT_DESC desc_face[1] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			library::vector<d3d11::input_element_descript> descript;
			descript.resize(1);
			descript[0] = d3d11::input_element_descript{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			_resources.create_resource<shader>("shader", L"VertexShader.hlsl", "main", "vs_5_0", L"PixelShader.hlsl", "main", "ps_5_0", descript);
		}
		//system
		auto pipeline = _scenes.create_render_system<framework::pipeline>();

		//object
		auto camera = _scenes.create_object();
		auto camera_component = camera->add_component<framework::camera>("camera", 60.f, 1280.f, 720.f, 0.2f, 500.f);
		auto camera_transform = camera->add_component<framework::transform>("transform");
		pipeline->set_camera(camera_component, camera_transform);

		auto object = _scenes.create_object();
		auto object_tramsform = object->add_component<framework::transform>("transform");
		//auto object_renderer = object->add_component<framework::renderer>("renderer");
		auto object_mesh = _resources.find_resource<framework::mesh>("mesh");

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
			//_editor.update();

			_scenes.render_system();
			//_editor.render();
			_graphic.render_start();
			_timer.sleep_frame();
		}
	}
}