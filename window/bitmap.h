#pragma once
#include "object.h"

namespace window {
	class dc;
	class bitmap final : public object {
	public:
		explicit bitmap(dc const& dc_, int const cx, int const cy) noexcept;
		explicit bitmap(HINSTANCE const hInst, LPCWSTR const name, int const cx, int const cy, UINT const fuLoad) noexcept;
		virtual ~bitmap(void) noexcept override = default;
	};
}