#pragma once
#include "window.h"
#include "graphic.h"
#include "frame.h"

#include "object_manager.h"
#include "component_manager.h"
#include "scene_manager.h"

#include "transform.h"
#include "camera.h"

namespace framework {
	class client : public library::singleton<client> {
		friend class library::singleton<client>;
		window _window;
		graphic _graphic;
		frame _frame;
	protected:
		object_manager _object_manager;
		component_manager _component_manager;
		scene_manager _scene_manager;
	public:
		inline explicit client(void) noexcept
			: _window(), _graphic(_window) {
			auto camera = create_object();
			camera->add_component("transform", &create_component<framework::transform>());
			//camera->add_component("camera", &create_component<framework::camera>());
			//regist_object("camera", camera);

			//create_scene();
		}
		inline explicit client(client const&) noexcept = delete;
		inline explicit client(client&&) noexcept = default;
		inline auto operator=(client const&) noexcept -> client & = delete;
		inline auto operator=(client&&) noexcept -> client & = default;
		inline ~client(void) noexcept = default;

		inline void execute(void) noexcept {
			for (;;) {
				_frame.update();
				_scene_manager.update();
				_graphic.render();
				_frame.sleep();
			}
		}

		inline auto create_object(void) noexcept {
			return _object_manager.create_object();
		}
		inline void regist_object(library::string const& name, framework::object* object) noexcept {
			_object_manager.regist_prototype(name, object);
		}
		inline auto clone_object(library::string const& name) noexcept {
			auto clone = _object_manager.clone_prototype(name);
			_scene_manager.add_object(clone);
		}

		template<typename type, typename... argument>
		inline auto create_component(void) noexcept -> type& {
			return _component_manager.create_component<type>();
		}

		inline auto create_scene(library::string path) noexcept {
			_scene_manager.create_scene(path);
		}
	};
}