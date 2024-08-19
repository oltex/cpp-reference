#pragma once
#include "object.h"

namespace window {
	class pen final : public object {
	public:
		inline explicit pen(int const iStyle, int const cWidth, COLORREF const color) noexcept {
			_hgdiobj = CreatePen(iStyle, cWidth, color);
		}
		inline virtual ~pen(void) noexcept override = default;
	};
}

