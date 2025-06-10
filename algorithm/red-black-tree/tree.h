#pragma once
#include "../../system/window/pen.h"
#include "../../system/window/brush.h"
#include "../../system/window/font.h"

class tree final {
public:
	inline explicit tree(void) noexcept
		: _red(RGB(255, 0, 0)),
		_black(RGB(0, 0, 0)),
		_line(PS_SOLID, 2, RGB(100, 100, 100)),
		_font(16, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
			L"Malgun Gothic") {
	}
	inline ~tree(void) noexcept = default;
public:
	window::brush _red;
	window::brush _black;
	window::pen _line;
	window::font _font;

	int _ellipse_size = 50;
};