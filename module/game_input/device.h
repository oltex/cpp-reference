#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include <GameInput.h>

namespace game_input {
	class declspec_dll device : public library::component<GameInput::v2::IGameInputDevice> {
		using base = library::component<GameInput::v2::IGameInputDevice>;
	public:
		inline explicit device(GameInput::v2::IGameInputDevice* device = nullptr) noexcept;
		inline explicit device(device const&) noexcept = delete;
		inline explicit device(device&&) noexcept = delete;
		inline auto operator=(device const&) noexcept -> device & = delete;
		inline auto operator=(device&&) noexcept -> device & = delete;
		inline ~device(void) noexcept = default;
	};
}