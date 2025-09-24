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
	inline auto reading::get_input_kind(void) noexcept -> GameInput::v2::GameInputKind {
		return _component->GetInputKind();
	}
	inline auto reading::get_mouse_state(void) noexcept -> mouse_state {
		mouse_state state;
		bool result = _component->GetMouseState(&state);
		return state;
	}
	inline auto reading::get_key_count(void) noexcept -> unsigned int {
		return _component->GetKeyCount();
	}
	inline auto reading::get_key_state(void) noexcept -> library::array<key_state, 16> {
		library::array<key_state, 16> key_state;
		_component->GetKeyState(_component->GetKeyCount(), key_state.data());
		return key_state;
	}
	bool declspec_dll operator==(reading& lhs, nullptr_t) noexcept {
		return nullptr == lhs.data();
	}
}