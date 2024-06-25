#include "dc.h"
#include "wnd.h"
#include "object.h"

namespace window {
	dc::dc(wnd const& wnd_, flag const flag_) noexcept
		: _wnd(&wnd_) {
		memset(&_ps, 0, sizeof(PAINTSTRUCT));
		switch (flag_) {
			using enum flag;
		case get_dc:
			_hdc = GetDC(_wnd->_hwnd);
			break;
		case begin_paint:
			_hdc = BeginPaint(_wnd->_hwnd, &_ps);
			break;
		}
	}
	dc::dc(dc const& rhs) noexcept
		: _hdc(CreateCompatibleDC(rhs._hdc)) {
		memset(&_ps, 0, sizeof(PAINTSTRUCT));
	}
	dc::~dc(void) noexcept {
		if (nullptr == _wnd)
			DeleteDC(_hdc);
		else
			switch (nullptr == _ps.hdc ? true : false) {
			case false:
				EndPaint(_wnd->_hwnd, &_ps);
				break;
			case true:
				ReleaseDC(_wnd->_hwnd, _hdc);
				break;
			}
	}
	void dc::select_object(object const& object_) noexcept {
		SelectObject(_hdc, object_._hgdiobj);
	}
}
