#include "bitmap.h"
#include "dc.h"
#include "wnd.h"

namespace window {
	bitmap::bitmap(dc const& dc_) noexcept {
		RECT rect = dc_._wnd->get_client_rect();
		_hgdiobj = CreateCompatibleBitmap(dc_._hdc, rect.right, rect.bottom);
	}
}
