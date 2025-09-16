#pragma once
#include "window.h"
#include "graphic.h"
#include "frame.h"
#include "component_manager.h"

#include "transform.h"
#include "camera.h"

namespace framework {
	class client {
		window _window;
		graphic _graphic;
		frame _frame;

		component_manager _component_manager;
	public:
		inline explicit client(void) noexcept
			: _window(), _graphic(_window) {
			_component_manager.add_component<transform>();
			_component_manager.add_component<camera>();
		}
		inline explicit client(client const&) noexcept = delete;
		inline explicit client(client&&) noexcept = default;
		inline auto operator=(client const&) noexcept -> client & = delete;
		inline auto operator=(client&&) noexcept -> client & = default;
		inline ~client(void) noexcept = default;

		inline void execute(void) noexcept {
			for (;;) {
				while (auto msg = ::window::peek_message()) {
					if (WM_QUIT == msg->message)
						return;
					else {
						::window::translate_message(*msg);
						::window::dispatch_message(*msg);
					}
				}
				_frame.update();
				_graphic.render();
				_frame.sleep();
			}
		}

	};
}