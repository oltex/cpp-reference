#include "input.h"

namespace framework {
	input::input(void) noexcept {
	}
	void input::update_state(void) noexcept {
		auto reading = _input.get_current_reading(GameInput::v2::GameInputKindMouse | GameInput::v2::GameInputKindKeyboard);
		if (nullptr != reading) {
			auto kind = reading.get_input_kind();
			if (GameInput::v2::GameInputKindMouse & kind) {
				_previous_mouse = _current_mouse;
				_current_mouse = reading.get_mouse_state();
			}
			if (GameInput::v2::GameInputKindKeyboard & kind) {
				auto count = reading.get_key_count();
				auto key_state = reading.get_key_state();

				_previous_keyboard = _current_keyboard;
				_current_keyboard.reset();
				for (auto& iter : key_state)
					_current_keyboard |= static_cast<key>(iter.virtualKey);
			}
		}
	}
	auto input::get_mouse_button(void) const noexcept -> mouse {
		return static_cast<mouse>(_current_mouse.buttons);
	}
	auto input::get_mouse_button_down(void) const noexcept -> mouse {
		return static_cast<mouse>(~_previous_mouse.buttons & _current_mouse.buttons);
	}
	auto input::get_mouse_button_up(void) const noexcept -> mouse {
		return static_cast<mouse>(_previous_mouse.buttons & ~_current_mouse.buttons);
	}
	auto input::get_mouse_wheel(void) const noexcept -> int {
		return static_cast<int>(_current_mouse.wheelY - _previous_mouse.wheelY);
	}
	auto input::get_mouse_move(void) noexcept -> library::pair<int, int> {
		return library::pair<int, int>(
			static_cast<int>(_current_mouse.positionX - _previous_mouse.positionX),
			static_cast<int>(_current_mouse.positionY - _previous_mouse.positionY)
		);
	}
	auto input::get_mouse_position(void) noexcept -> library::pair<int, int> {
		return library::pair<int, int>(
			static_cast<int>(_current_mouse.absolutePositionX),
			static_cast<int>(_current_mouse.absolutePositionY)
		);
	}
	auto input::get_keyboard_button(void) noexcept -> keyboard const& {
		return _current_keyboard;
	}
	auto input::get_keyboard_button_down(void) noexcept -> keyboard {
		return ~_previous_keyboard & _current_keyboard;
	}
	auto input::get_keyboard_button_up(void) noexcept -> keyboard {
		return _previous_keyboard & ~_current_keyboard;
	}
}