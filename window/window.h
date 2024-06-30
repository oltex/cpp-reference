#pragma once
#include "wnd.h"
#include "cls.h"
#include "sct.h"

#include "cursor.h"
#include "icon.h"

#include "dc.h"
#include "bitmap.h"
#include "brush.h"
#include "pen.h"

namespace window {
	inline auto message(void) noexcept -> int {
		MSG msg;
		while (GetMessageW(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		return (int)msg.wParam;
	};
}