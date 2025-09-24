#include "input.h"
#include "library/container/array.h"

namespace framework {
	input::input(void) noexcept {
		GameInput::v2::GameInputCreate(&_game_input);
	}
	input::~input(void) noexcept {
		_game_input->Release();
	}

	void input::update_state(void) noexcept {
		GameInput::v2::IGameInputReading* reading;
		if (SUCCEEDED(_game_input->GetCurrentReading(GameInput::v2::GameInputKindMouse | GameInput::v2::GameInputKindKeyboard, nullptr, &reading))) {
			auto kind = reading->GetInputKind();
			if (GameInput::v2::GameInputKindMouse & kind) {
				_previous_mouse = _current_mouse;
				reading->GetMouseState(&_current_mouse);
			}
			if (GameInput::v2::GameInputKindKeyboard & kind) {
				_previous_keyboard = _current_keyboard;
				_current_keyboard.reset();

				library::array<GameInput::v2::GameInputKeyState, 16> state;
				reading->GetKeyState(reading->GetKeyCount(), state.data());
				for (auto& iter : state)
					_current_keyboard |= static_cast<key>(iter.virtualKey);
			}
		}
	}
	auto input::get_mouse_button(void) const noexcept -> framework::mouse {
		return static_cast<framework::mouse>(_current_mouse.buttons);
	}
	auto input::get_mouse_button_down(void) const noexcept -> framework::mouse {
		return static_cast<framework::mouse>(~_previous_mouse.buttons & _current_mouse.buttons);
	}
	auto input::get_mouse_button_up(void) const noexcept -> framework::mouse {
		return static_cast<framework::mouse>(_previous_mouse.buttons & ~_current_mouse.buttons);
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