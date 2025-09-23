#include "input.h"

namespace game_input {
	inline input::input(void) noexcept {
		GameInput::v2::GameInputCreate(&_component);
	}

	inline auto input::get_current_reading(GameInput::v2::GameInputKind kind) noexcept -> reading {
		GameInput::v2::IGameInputReading* current;
		_component->GetCurrentReading(kind, nullptr, &current);
		return reading(current);
	}
	inline auto input::get_previous_reading(reading& current, GameInput::v2::GameInputKind kind) noexcept -> reading {
		GameInput::v2::IGameInputReading* previous;
		_component->GetPreviousReading(current.data(), kind, nullptr, &previous);
		return reading(previous);
	}
	inline auto input::get_next_reading(reading& current, GameInput::v2::GameInputKind kind) noexcept -> reading {
		GameInput::v2::IGameInputReading* next;
		_component->GetNextReading(current.data(), kind, nullptr, &next);
		return reading(next);
	}
}