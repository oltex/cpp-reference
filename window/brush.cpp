#include "brush.h"

namespace window{
	brush::brush(COLORREF const color) noexcept {
		_hgdiobj = CreateSolidBrush(color);
	}
}
