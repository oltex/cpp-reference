#include "bitmap.h"
#include "dc.h"
#include "wnd.h"

namespace window {
	bitmap::bitmap(dc const& dc_, int const cx, int const cy) noexcept {
		_hgdiobj = CreateCompatibleBitmap(dc_._hdc, cx, cy);
	}
	bitmap::bitmap(HINSTANCE const hInst, LPCWSTR const name, int const cx, int const cy, UINT const fuLoad) noexcept {
		_hgdiobj = static_cast<HBITMAP>(LoadImageW(hInst, name, IMAGE_BITMAP, cx, cy, fuLoad));
	}
}
