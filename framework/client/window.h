#pragma once
#include "module/window/window.h"
#include "library/pattern/singleton.h"
#include "library/system/thread.h"

namespace framework {
	class window : public winapi::handle, public library::singleton<window> {
		friend class library::singleton<window>;
		friend class graphic;
		friend class editor;
		library::thread _thread;

		explicit window(void) noexcept;
		explicit window(window const&) noexcept = delete;
		explicit window(window&&) noexcept = delete;
		auto operator=(window const&) noexcept -> window & = delete;
		auto operator=(window&&) noexcept -> window & = delete;
		~window(void) noexcept = default;
	public:
		bool check_exit(void) noexcept;
		void execute(void) noexcept;
		virtual bool procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept override;
	};
}