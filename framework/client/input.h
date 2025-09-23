#pragma once
#include "library/system/component.h"
#pragma comment(lib, "module/game_input/binary/game_input.lib")
#include "module/game_input/game_input.h"

namespace framework {
	class input {
		game_input::input _input;
		enum mouse {
			left, right, wheel
		};
	public:
		explicit input(void) noexcept;
		inline explicit input(input const&) noexcept = delete;
		inline explicit input(input&&) noexcept = delete;
		inline auto operator=(input const&) noexcept -> input & = delete;
		inline auto operator=(input&&) noexcept -> input & = delete;
		~input(void) noexcept = default;

		void update_current(void) noexcept;


	};
}