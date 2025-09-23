#include "device.h"

namespace game_input {
	inline device::device(GameInput::v2::IGameInputDevice* device) noexcept
		: base(device) {

	}
}