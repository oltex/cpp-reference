#pragma once
#include "module/window/instance.h"
#include "module/window/class.h"
#include "module/window/struct.h"

#include "module/directx/device.h"
#include "module/directx/swap_chain.h"

namespace framework {
	class client {
		window::handle _handle;
		
	public:
		inline explicit client(void) noexcept {
			{
				window::_class _class;
				_class.class_name(L"window");
				_class.style(CS_HREDRAW | CS_VREDRAW);
				_class.cursor(window::load_cursor(IDC_ARROW));
				_class.regist();

				window::_struct _struct;
				_struct.class_name(L"window");
				_struct.style(WS_OVERLAPPEDWINDOW);
				_struct.x(CW_USEDEFAULT);
				_struct.width(CW_USEDEFAULT);
				_struct.create_window(_handle);
			}
			_handle.show(true);
			_handle.update();
		}
		inline explicit client(client const&) noexcept = delete;
		inline explicit client(client&&) noexcept = default;
		inline auto operator=(client const&) noexcept -> client & = delete;
		inline auto operator=(client&&) noexcept -> client & = default;
		inline ~client(void) noexcept = default;

		inline void execute(void) noexcept {
			for (;;) {
				while (auto msg = window::peek_message()) {
					if (WM_QUIT == msg->message)
						return;
					else {
						window::translate_message(*msg);
						window::dispatch_message(*msg);
					}
				}
			}
		}
	};
}