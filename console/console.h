#pragma once
#include "Windows.h"

class console final {
public:
	inline explicit console(void) noexcept = default;
	inline ~console(void) noexcept = default;
public:
	inline void allocate(void) noexcept {
		AllocConsole();
	}
	inline void deallocate(void) noexcept {
		FreeConsole();
	}

	inline void set_mode(unsigned long const handle, unsigned long const mode) const noexcept {
		SetConsoleMode(GetStdHandle(handle), mode);
	}
	inline auto get_mode(unsigned long const handle) const noexcept -> unsigned long {
		unsigned long mode;
		GetConsoleMode(GetStdHandle(handle), &mode);
		return mode;
	}
	inline void set_cursor(CONSOLE_CURSOR_INFO const& info) const noexcept {
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}
};