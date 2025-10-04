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
		_editor(editor::instance()),
		_audio(audio::instance()),
		_input(input::instance()),
		_scenes(scenes::instance()),
		_objects(objects::instance()),
		_components(components::instance()),
		_resources(resources::instance()) {

		_scenes.create_scene("");
		_scenes.update_system();

		//resource

		_resources.create_resource<texture>("texture", L"images.png");
		auto material = _resources.create_resource<framework::material>("material");
		material->add_texture("texture");

		auto mesh = _resources.create_resource<framework::mesh>("mesh");
		struct vertex_face {
			dmath::float3 _position;
			dmath::float2 _texcoord;
		};
		library::vector<vertex_face> vertex{
			{ {-0.5f,  0.5f, 0.f}, {0.f, 0.f} },
			{ { 0.5f,  0.5f, 0.f}, {1.f, 0.f} },
			{ { 0.5f, -0.5f, 0.f}, {1.f, 1.f} },
			{ {-0.5f, -0.5f, 0.f}, {0.f, 1.f} },
		};
		library::vector<unsigned short> index{
			0, 1, 2, 0, 2, 3
		};
		mesh->add_primitive(vertex, index, DXGI_FORMAT_R16_UINT);

		library::vector<d3d11::input_element_descript> descript{
			d3d11::input_element_descript{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			d3d11::input_element_descript{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		_resources.create_resource<shader>("shader", L"VertexShader.hlsl", "main", "vs_5_0", L"PixelShader.hlsl", "main", "ps_5_0", descript);
		auto model = _resources.create_resource<framework::mesh>("model", "HatKid.glb");



		//system
		auto pipeline = _scenes.create_render_system<framework::pipeline>();

		//object
		auto camera = _scenes.create_object();
		auto camera_component = camera->add_component<framework::camera>("camera", 60.f, 1280.f, 720.f, 0.2f, 500.f);
		auto camera_transform = camera->add_component<framework::transform>("transform");
		pipeline->set_camera(camera_component, camera_transform);
		auto object = _scenes.create_object();
		auto object_tramsform = object->add_component<framework::transform>("transform");
		auto object_renderer = object->add_component<framework::renderer>("renderer");
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