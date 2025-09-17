#pragma once
#include "client.h"
#include "module/window/window.h"
#include "library/system/thread.h"

namespace framework {
	class window : public winapi::handle {
		library::thread _thread;
	public:
		inline explicit window(void) noexcept {
			_thread.begin(&window::execute, 0, this);
			while (nullptr == _hwnd) {}
		}
		inline explicit window(window const&) noexcept = delete;
		inline explicit window(window&&) noexcept = delete;
		inline auto operator=(window const&) noexcept -> window & = delete;
		inline auto operator=(window&&) noexcept -> window & = delete;
		inline ~window(void) noexcept = default;

		inline void execute(void) noexcept {
			winapi::_class _class;
			_class.class_name(L"window");
			_class.style(CS_HREDRAW | CS_VREDRAW);
			_class.cursor(::winapi::load_cursor(IDC_ARROW));
			_class.regist();
			winapi::_struct _struct;
			_struct.class_name(L"window");
			_struct.style(WS_OVERLAPPEDWINDOW);
			_struct.x(CW_USEDEFAULT);
			_struct.width(CW_USEDEFAULT);
			_struct.create_window(*this);
			show(true);

			while (auto msg = winapi::get_message()) {
				winapi::translate_message(*msg);
				winapi::dispatch_message(*msg);
			}
		}
	};
}