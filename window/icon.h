#pragma once
#include <Windows.h>

namespace window {
	class icon final {
		friend class cls;
	public:
		inline auto destroy(void) noexcept -> BOOL {
			return DestroyIcon(_hicon);
		}
		inline auto draw(HDC const dc, int x, int y) noexcept {
			DrawIcon(dc, x, y, _hicon);
		}
		inline void load(HINSTANCE const hInst, LPCWSTR const name, UINT const type, int const cx, int const cy, UINT const fuLoad) noexcept {
			_hicon = static_cast<HICON>(LoadImageW(hInst, name, type, cx, cy, fuLoad));
		};
	private:
		HICON _hicon = nullptr;
	};
}