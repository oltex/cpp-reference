#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "reading.h"
#include <GameInput.h>

namespace game_input {
	class declspec_dll input : public library::component<GameInput::v2::IGameInput> {
	public:
		inline explicit input(void) noexcept;
		inline explicit input(input const&) noexcept = delete;
		inline explicit input(input&&) noexcept = delete;
		inline auto operator=(input const&) noexcept -> input & = delete;
		inline auto operator=(input&&) noexcept -> input & = delete;
		inline ~input(void) noexcept = default;

		inline auto get_current_reading(GameInput::v2::GameInputKind kind) noexcept -> reading;
		inline auto get_previous_reading(reading& current, GameInput::v2::GameInputKind kind)noexcept -> reading;
		inline auto get_next_reading(reading& current, GameInput::v2::GameInputKind kind)noexcept -> reading;
	};
}