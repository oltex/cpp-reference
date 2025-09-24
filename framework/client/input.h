#pragma once
#include "library/system/component.h"
#include "library/container/pair.h"
#pragma comment(lib, "module/game_input/binary/game_input.lib")
#include "module/game_input/game_input.h"
#include <bitset>

namespace framework {
	enum mouse {
		left = 0x00000001, right = 0x00000002, middle = 0x00000004
	};
	enum key {
		a = 0x41, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
	};

	class input {
		game_input::input _input;
		game_input::mouse_state _current_mouse_state;
		game_input::mouse_state _previous_mouse_state;

		std::bitset<255> _current_key_state;
		std::bitset<255> _previous_key_state;
	public:


		explicit input(void) noexcept;
		inline explicit input(input const&) noexcept = delete;
		inline explicit input(input&&) noexcept = delete;
		inline auto operator=(input const&) noexcept -> input & = delete;
		inline auto operator=(input&&) noexcept -> input & = delete;
		~input(void) noexcept = default;

		void update_state(void) noexcept;
		auto get_mouse_button(void) const noexcept -> mouse;
		auto get_mouse_button_down(void) const noexcept -> mouse;
		auto get_mouse_button_up(void) const noexcept -> mouse;
		auto get_mouse_wheel(void) const noexcept -> int;
		auto get_mouse_move(void) noexcept -> library::pair<int, int>;
		auto get_mouse_position(void) noexcept -> library::pair<int, int>;
	};

	inline auto operator|(std::bitset<255> const& lhs, key rhs) -> std::bitset<255> {
		std::bitset<255> result(lhs);
		result.set(rhs);
		return result;
	}
	inline auto operator|=(std::bitset<255>& lhs, key rhs) -> std::bitset<255>& {
		lhs.set(rhs);
		return lhs;
	}
	inline auto operator&(std::bitset<255> const& lhs, key rhs) -> std::bitset<255> {
		std::bitset<255> result;
		if (lhs.test(rhs))
			result.set(rhs);
		return result;
	}
	inline auto operator&=(std::bitset<255>& lhs, key rhs) -> std::bitset<255>& {
		bool keep = lhs.test(static_cast<size_t>(rhs));
		lhs.reset();
		if (keep) 
			lhs.set(static_cast<size_t>(rhs));
		return lhs;
	}
}