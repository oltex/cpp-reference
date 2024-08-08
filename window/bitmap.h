#pragma once
#include "object.h"

namespace window {
	class dc;
	class bitmap final : public object {
	public:
		explicit bitmap(dc const& dc_, int const cx, int const cy) noexcept;
		inline explicit bitmap(HINSTANCE const hInst, LPCWSTR const name, int const cx, int const cy, UINT const fuLoad) noexcept {
			_hgdiobj = static_cast<HBITMAP>(LoadImageW(hInst, name, IMAGE_BITMAP, cx, cy, fuLoad));
		}
		virtual ~bitmap(void) noexcept override = default;
	};
}