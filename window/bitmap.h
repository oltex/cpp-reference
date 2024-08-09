#pragma once
#include "object.h"

namespace window {
	class bitmap final : public object {
	public:
		inline explicit bitmap(HBITMAP const hbitmap) noexcept {
			_hgdiobj = hbitmap;
		}
		inline explicit bitmap(HINSTANCE const hInst, LPCWSTR const name, int const cx, int const cy, UINT const fuLoad) noexcept {
			_hgdiobj = static_cast<HBITMAP>(LoadImageW(hInst, name, IMAGE_BITMAP, cx, cy, fuLoad));
		}
		virtual ~bitmap(void) noexcept override = default;
	};
}