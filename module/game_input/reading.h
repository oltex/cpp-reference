#pragma once
#include "library/define.h"
#include "library/system/component.h"
#include "library/container/array.h"
#include "device.h"
#include "mouse_state.h"
#include "key_state.h"
#include <GameInput.h>

namespace game_input {
	class declspec_dll reading : public library::component<GameInput::v2::IGameInputReading> {
	public:
		inline explicit reading(GameInput::v2::IGameInputReading* read = nullptr) noexcept;
		inline explicit reading(reading const&) noexcept = delete;
		inline explicit reading(reading&&) noexcept = delete;
		inline auto operator=(reading const&) noexcept -> reading & = delete;
		inline auto operator=(reading&&) noexcept -> reading & = delete;
		inline ~reading(void) noexcept = default;

		inline auto get_device(void) noexcept -> device;
		inline auto get_input_kind(void) noexcept -> GameInput::v2::GameInputKind;
		inline auto get_mouse_state(void) noexcept -> mouse_state;
		inline auto get_key_count(void) noexcept -> unsigned int;
		inline auto get_key_state(void) noexcept -> library::array<key_state, 16>;

		inline friend bool declspec_dll operator==(reading& lhs, nullptr_t) noexcept;
	};
}