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
	class client {
		window _window;
		graphic _graphic;
		frame _frame;
	protected:
		component_manager _component_manager;
		scene_manager _scene_manager;
		object_manager _object_manager;
	public:
		inline explicit client(void) noexcept
			: _window(), _graphic(_window) {
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
	};
}