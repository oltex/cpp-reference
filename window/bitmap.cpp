#include "bitmap.h"
#include "dc.h"
#include "wnd.h"

namespace window {
	bitmap::bitmap(dc const& dc_, int const cx, int const cy) noexcept {
		_hgdiobj = CreateCompatibleBitmap(dc_._hdc, cx, cy);
	}

}
