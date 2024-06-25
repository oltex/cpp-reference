#pragma once
#include "object.h"

namespace window {
	class pen final : public object {
	public:
		explicit pen(int const iStyle, int const cWidth, COLORREF const color) noexcept;
		virtual ~pen(void) noexcept override = default;
	};
}

