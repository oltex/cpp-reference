#pragma once
#include "library/system/component.h"
#include "GameInput.h"

namespace framework {
	class input : public library::component< GameInput::v2::IGameInput>{
		GameInput::v2::IGameInputReading* reading = nullptr;

		inline explicit input(void) noexcept {
			GameInput::v2::GameInputCreate(&_component);
		}
	};
}