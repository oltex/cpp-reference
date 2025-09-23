#include "input.h"

namespace framework {
	input::input(void) noexcept {
	}
	void input::update_current(void) noexcept {
		auto reading = _input.get_current_reading(GameInput::v2::GameInputKindMouse /*| GameInput::v2::GameInputKindKeyboard*/);
		if (nullptr != reading) {
			auto mouse_state = reading.get_mouse_state();
			
			game_input::key_state key_state[16];
			int a = 10;
		}
	}
}