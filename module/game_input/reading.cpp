#include "reading.h"

namespace game_input {
	inline reading::reading(GameInput::v2::IGameInputReading* read) noexcept
		: component(read) {
	}
	inline auto reading::get_device(void) noexcept -> game_input::device {
		GameInput::v2::IGameInputDevice* device;
		_component->GetDevice(&device);
		return game_input::device(device);
	}
	inline auto reading::get_mouse_state(void) noexcept -> mouse_state {
		mouse_state state;
		_component->GetMouseState(&state);
		return state;
	}
	inline auto reading::get_key_count(void) noexcept -> unsigned int {
		return _component->GetKeyCount();
	}
	inline auto reading::get_key_state(void) noexcept {
		_component->GetKeyState();
	}
	 bool declspec_dll operator==(reading& lhs, nullptr_t) noexcept {
		return nullptr == lhs.data();
	}
}