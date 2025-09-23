#pragma once
#include "library/system/component.h"
#include "GameInput.h"
#include "mouse_state.h"

namespace game_input {
	class reading : public library::component<GameInput::v2::IGameInputReading> {
	public:
		explicit reading(GameInput::v2::IGameInputReading* read = nullptr) noexcept;
		inline explicit reading(reading const&) noexcept = delete;
		inline explicit reading(reading&&) noexcept = delete;
		inline auto operator=(reading const&) noexcept -> reading & = delete;
		inline auto operator=(reading&&) noexcept -> reading & = delete;
		inline ~reading(void) noexcept = default;

		inline auto get_key_count(void) noexcept -> unsigned int;
		inline auto get_key_state(void) noexcept;
		inline auto get_mouse_state(void) noexcept -> mouse_state;
	};
}