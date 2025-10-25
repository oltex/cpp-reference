#include "pipeline.h"
#include "graphic.h"

#include "resource.h"
#include "shader.h"
#include "mesh.h"
#include "scene.h"

struct VS_CONSTANT_BUFFER {
	dmath::float4x4 _world_matrix;
};

namespace framework {
	pipeline::pipeline(void) noexcept
		: _view_port(0, 0, 1424, 720, 0.f, 1.f) {
		auto& device = graphic::instance()._device;
		auto& device_context = graphic::instance()._device_context;
		auto& swap_chain = graphic::instance()._swap_chain;
		{
			auto texture = swap_chain.get_buffer_texture_2d();
			_render_target_view = device.create_render_target_view(texture);
		}
		{
			auto texture = device.create_texture_2d(d3d11::texture_2d_descript(1424, 720, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL, 0, 0));
			_depth_stencil_view = device.create_depth_stencil_view(texture);
		}
	}
	void pipeline::update(void) noexcept {
		//auto& device_context = graphic::instance()._device_context;
		//device_context.set_view_port(_view_port);
		//device_context.set_render_target(_render_target_view, nullptr);
		//float color[4]{ 0.1f/*rand() / (float)RAND_MAX*/, 0.1f/*rand() / (float)RAND_MAX*/, 0.1f/*rand() / (float)RAND_MAX*/, 1.f };
		//device_context.clear_render_target_view(_render_target_view, color);
		//device_context.clear_depth_stencil_view(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		//camera
		//if (_camera.expire()) {
		//	auto camera = _camera.lock();
		//	auto transform = _camera_transform.lock();
		//	device_context.set_vertex_shader_constant_buffer(0, 1, &camera->buffer().data());
		//	device_context.set_vertex_shader_constant_buffer(1, 1, &transform->buffer().data());

		//	auto resource = device_context.map(camera->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0);
		//	auto view_matrix = transform->matrix().inverse().transpose();
		//	library::memory_copy(resource.pData, &view_matrix, sizeof(dmath::float4x4));
		//	auto proj_matrix = camera->matrix().transpose();
		//	library::memory_copy(reinterpret_cast<char*>(resource.pData) + sizeof(dmath::float4x4), &proj_matrix, sizeof(dmath::float4x4));
		//	device_context.unmap(camera->buffer(), 0);
		//}

		//for (auto& renderer : _renderer) {
		//	auto renderer2 = renderer.lock();
		//	for (auto& draw_item : renderer2->get_draw_item()) {
		//		device_context.set_input_layout(draw_item._material[0]->_shader->_input_layout);
		//		device_context.set_vertex_shader(draw_item._material[0]->_shader->_vertex_shader);
		//		device_context.set_pixel_shader(draw_item._material[0]->_shader->_pixel_shader);
		//		device_context.set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//		//device_context.set_pixel_shader_resource(0, 1, &draw_item._material[0]->_texture[0]->_srv.data());

		//		auto resource = device_context.map(draw_item._transform->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0);
		//		auto matrix = draw_item._transform->matrix().transpose().store();
		//		library::memory_copy(resource.pData, &matrix, sizeof(dmath::float4x4));
		//		device_context.unmap(draw_item._transform->buffer(), 0);

		//		draw_item._mesh->render_primitive(0);
		//	}
		//}

	}

	void pipeline::add_component(library::string const& key, library::vector<library::intrusive::share_pointer<component, 0>> const& component) noexcept {
		if ("camera" == key) {
			_camera = component[0];
			_camera_transform = component[1];
		}
		if ("object" == key) {
			_renderer.emplace_back(component[0]);
		}
	}
}
