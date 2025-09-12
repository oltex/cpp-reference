#pragma once
#include "client.h"

#include "module/window/instance.h"
#include "module/window/class.h"
#include "module/window/struct.h"

namespace framework {
	class window : public ::window::handle {
	public:
		inline explicit window(void) noexcept {
			::window::_class _class;
			_class.class_name(L"window");
			_class.style(CS_HREDRAW | CS_VREDRAW);
			_class.cursor(::window::load_cursor(IDC_ARROW));
			_class.regist();
			::window::_struct _struct;
			_struct.class_name(L"window");
			_struct.style(WS_OVERLAPPEDWINDOW);
			_struct.x(CW_USEDEFAULT);
			_struct.width(CW_USEDEFAULT);
			_struct.create_window(*this);

			show(true);
			update();
		}
	};
}