#pragma once
#include "library/pattern/singleton.h"
#include "library/system/component.h"
#include "library/container/pair.h"
#include "library/container/bit_set.h"
#include <GameInput.h>
#include <bitset>

namespace framework {
	enum mouse {
		left = 0x00000001, right = 0x00000002, middle = 0x00000004
	};
	enum key {
		a = 0x41, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
	};
	class input : public library::singleton<input> {
		friend class library::singleton<input>;
		using mouse = GameInput::v2::GameInputMouseState;
		using keyboard = library::bit_set<255>;

		GameInput::v2::IGameInput* _game_input;
		mouse _current_mouse;
		mouse _previous_mouse;
		keyboard _current_keyboard;
		keyboard _previous_keyboard;

		explicit input(void) noexcept;
		explicit input(input const&) noexcept = delete;
		explicit input(input&&) noexcept = delete;
		auto operator=(input const&) noexcept -> input & = delete;
		auto operator=(input&&) noexcept -> input & = delete;
		~input(void) noexcept;
	public:
		void update_state(void) noexcept;
		auto get_mouse_button(void) const noexcept -> framework::mouse;
		auto get_mouse_button_down(void) const noexcept -> framework::mouse;
		auto get_mouse_button_up(void) const noexcept -> framework::mouse;
		auto get_mouse_wheel(void) const noexcept -> int;
		auto get_mouse_move(void) noexcept -> library::pair<int, int>;
		auto get_mouse_position(void) noexcept -> library::pair<int, int>;
		auto get_keyboard_button(void) noexcept -> keyboard const&;
		auto get_keyboard_button_down(void) noexcept -> keyboard;
		auto get_keyboard_button_up(void) noexcept -> keyboard;
	};

	inline auto operator|(library::bit_set<255> const& lhs, key rhs) -> library::bit_set<255> {
		library::bit_set<255> result(lhs);
		result.set(rhs, true);
		return result;
	}
	inline auto operator|(key rhs, library::bit_set<255> const& lhs) -> library::bit_set<255> {
		library::bit_set<255> result(lhs);
		result.set(rhs, true);
		return result;
	}
	inline auto operator|=(library::bit_set<255>& lhs, key rhs) -> library::bit_set<255>& {
		lhs.set(rhs, true);
		return lhs;
	}
	inline auto operator&(library::bit_set<255> const& lhs, key rhs) -> library::bit_set<255> {
		library::bit_set<255> result;
		if (lhs.test(rhs))
			result.set(rhs, 1);
		return result;
	}
	inline auto operator&(key rhs, library::bit_set<255> const& lhs) -> library::bit_set<255> {
		library::bit_set<255> result;
		if (lhs.test(rhs))
			result.set(rhs, 1);
		return result;
	}
	inline auto operator&=(library::bit_set<255>& lhs, key rhs) -> library::bit_set<255>& {
		bool keep = lhs.test(static_cast<size_t>(rhs));
		lhs.reset();
		if (keep)
			lhs.set(rhs, 1);
		return lhs;
	}
}