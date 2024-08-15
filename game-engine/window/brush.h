#pragma once
#include "object.h"

namespace window {
	class brush final : public object {
	public:
		inline explicit brush(COLORREF const color) noexcept {
			_hgdiobj = CreateSolidBrush(color);
		};
		virtual ~brush(void) noexcept override = default;
	};
}

