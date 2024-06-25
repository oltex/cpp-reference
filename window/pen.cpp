#include "pen.h"

namespace window {
	pen::pen(int const iStyle, int const cWidth, COLORREF const color) noexcept {
		_hgdiobj = CreatePen(iStyle, cWidth, color);
	}
}