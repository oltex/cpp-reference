#pragma once
#include "scene.h"

namespace framework {
	class scene_manager {
		scene* _current_scene;
		scene* _previous_scene;
	public:
		inline explicit scene_manager(void) noexcept = default;
		inline explicit scene_manager(scene_manager const&) noexcept = delete;
		inline explicit scene_manager(scene_manager&&) noexcept = delete;
		inline auto operator=(scene_manager const&) noexcept -> scene_manager & = delete;
		inline auto operator=(scene_manager&&) noexcept -> scene_manager & = delete;
		inline virtual ~scene_manager(void) noexcept = default;

		inline void update(void) noexcept {
			if (nullptr != _previous_scene)
				_current_scene = _previous_scene;
			if (nullptr != _current_scene)
				_current_scene->update();
		}

		inline void create_scene(library::string path) noexcept {
			auto _previous_scene = new framework::scene;
		}
		inline auto add_object(object* object) noexcept {
			_current_scene->add_object();
		}
		inline auto add_system(void) noexcept {
			_current_scene->add_object();
		}
	};
}