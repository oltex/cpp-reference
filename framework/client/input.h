#pragma once
#include "library/system/component.h"
#include "library/container/pair.h"
#pragma comment(lib, "module/game_input/binary/game_input.lib")
#include "module/game_input/game_input.h"
#include <bitset>

namespace framework {
	class input {
		game_input::input _input;
		game_input::mouse_state _current_mouse_state;
		game_input::mouse_state _previous_mouse_state;

		std::bitset<255> _current_key_board_state;
		std::bitset<255> _previous_key_board_state;

		enum mouse_button {
			left = 0x00000001, right = 0x00000002, middle = 0x00000004
		};
	public:
		explicit input(void) noexcept;
		inline explicit input(input const&) noexcept = delete;
		inline explicit input(input&&) noexcept = delete;
		inline auto operator=(input const&) noexcept -> input & = delete;
		inline auto operator=(input&&) noexcept -> input & = delete;
		~input(void) noexcept = default;

		void update_state(void) noexcept;
		auto get_mouse_button(void) const noexcept -> mouse_button;
		auto get_mouse_button_down(void) const noexcept -> mouse_button;
		auto get_mouse_button_up(void) const noexcept -> mouse_button;
		auto get_mouse_wheel(void) const noexcept -> int;
		auto get_mouse_move(void) noexcept -> library::pair<int, int>;
		auto get_mouse_position(void) noexcept -> library::pair<int, int>;
	};
}