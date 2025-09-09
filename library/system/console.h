#pragma once
#include <Windows.h>

namespace library {
	class console final {
	public:
		inline explicit console(void) noexcept = default;
		inline explicit console(console const&) noexcept = delete;
		inline explicit console(console&&) noexcept = delete;
		inline auto operator=(console const&) noexcept -> console & = delete;
		inline auto operator=(console&&) noexcept -> console & = delete;
		inline ~console(void) noexcept = default;

		inline void allocate(void) noexcept {
			::AllocConsole();
		}
		inline void deallocate(void) noexcept {
			::FreeConsole();
		}
		inline void mode(unsigned long const handle, unsigned long const mode) const noexcept {
			::SetConsoleMode(GetStdHandle(handle), mode);
		}
		inline auto mode(unsigned long const handle) const noexcept -> unsigned long {
			unsigned long mode;
			::GetConsoleMode(GetStdHandle(handle), &mode);
			return mode;
		}
		inline void cursor(CONSOLE_CURSOR_INFO const& info) const noexcept {
			::SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		}
	};
}