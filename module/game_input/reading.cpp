#include "reading.h"

namespace game_input {
	reading::reading(GameInput::v2::IGameInputReading* read) noexcept
		: component(read) {
	}
	inline auto reading::get_key_count(void) noexcept -> unsigned int {
		return _component->GetKeyCount();
	}
	inline auto reading::get_key_state(void) noexcept {
		_component->GetKeyState();
	}
	inline auto reading::get_mouse_state(void) noexcept -> mouse_state {
		mouse_state state;
		_component->GetMouseState(&state);
		return state;
	}
}