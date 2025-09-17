#pragma once
#include "client.h"
#include "my_scene.h"
#include "object.h"

class my_client : public framework::client {
	inline explicit my_client(void) noexcept {

		auto camera = new framework::object;
		camera->add_component("transform", &_component_manager.create_component<framework::transform>());
		camera->add_component("camera", &_component_manager.create_component<framework::camera>());
		_object_manager.regist_prototype("camera", camera);
		
		_scene_manager.load_scene(new my_scene);
	}
	inline explicit my_client(my_client const&) noexcept = delete;
	inline explicit my_client(my_client&&) noexcept = default;
	inline auto operator=(my_client const&) noexcept -> my_client & = delete;
	inline auto operator=(my_client&&) noexcept -> my_client & = default;
	inline ~my_client(void) noexcept = default;
};