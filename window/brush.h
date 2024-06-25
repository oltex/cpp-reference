#pragma once
#include "object.h"

namespace window {
	class brush final : public object {
	public:
		explicit brush(COLORREF const color) noexcept;
		virtual ~brush(void) noexcept override = default;
	};
}

