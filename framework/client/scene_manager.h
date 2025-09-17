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

		inline void load_scene(framework::scene* scene) noexcept {
			_previous_scene = scene;
		}
		inline void update(void) noexcept {
			if (nullptr != _previous_scene)
				_current_scene = _previous_scene;
			if (nullptr != _current_scene)
				_current_scene->update();
		}
	};
}