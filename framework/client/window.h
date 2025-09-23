#pragma once
#include "module/window/window.h"
#include "library/system/thread.h"

namespace framework {
	class window : public winapi::handle {
		library::thread _thread;
	public:
		explicit window(void) noexcept;
		inline explicit window(window const&) noexcept = delete;
		inline explicit window(window&&) noexcept = delete;
		inline auto operator=(window const&) noexcept -> window & = delete;
		inline auto operator=(window&&) noexcept -> window & = delete;
		inline ~window(void) noexcept = default;

		bool is_exit(void) noexcept;
		void execute(void) noexcept;
	};
}