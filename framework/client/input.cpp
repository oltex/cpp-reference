#include "input.h"

namespace framework {
	input::input(void) noexcept {
	}
	void input::update_state(void) noexcept {
		auto reading = _input.get_current_reading(GameInput::v2::GameInputKindMouse | GameInput::v2::GameInputKindKeyboard);
		if (nullptr != reading) {
			auto kind = reading.get_input_kind();
			if (GameInput::v2::GameInputKindMouse & kind) {
				_previous_mouse_state = _current_mouse_state;
				_current_mouse_state = reading.get_mouse_state();
			}
			if (GameInput::v2::GameInputKindKeyboard & kind) {
				reading.get_key_state();
			}

			int a = 10;
		}
	}
	auto input::get_mouse_button(void) const noexcept -> mouse_button {
		return static_cast<mouse_button>(_current_mouse_state.buttons);
	}
	auto input::get_mouse_button_down(void) const noexcept -> mouse_button {
		return static_cast<mouse_button>(~_previous_mouse_state.buttons & _current_mouse_state.buttons);
	}
	auto input::get_mouse_button_up(void) const noexcept -> mouse_button {
		return static_cast<mouse_button>(_previous_mouse_state.buttons & ~_current_mouse_state.buttons);
	}
	auto input::get_mouse_wheel(void) const noexcept -> int {
		return static_cast<int>(_current_mouse_state.wheelY - _previous_mouse_state.wheelY);
	}
	auto input::get_mouse_move(void) noexcept -> library::pair<int, int> {
		return library::pair<int, int>(
			static_cast<int>(_current_mouse_state.positionX - _previous_mouse_state.positionX),
			static_cast<int>(_current_mouse_state.positionY - _previous_mouse_state.positionY)
		);
	}
	auto input::get_mouse_position(void) noexcept -> library::pair<int, int> {
		return library::pair<int, int>(
			static_cast<int>(_current_mouse_state.absolutePositionX),
			static_cast<int>(_current_mouse_state.absolutePositionY)
		);
	}
}