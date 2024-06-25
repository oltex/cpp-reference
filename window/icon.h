#pragma once
#include <Windows.h>

namespace window {
	class registry;
	class icon final {
		friend class registry;
	public:
		explicit icon(void) noexcept = default;
	public:
		inline auto destroy(void) noexcept -> BOOL {
			return DestroyIcon(_icon);
		}
		inline auto draw(HDC const dc, int x, int y) noexcept {
			DrawIcon(dc, x, y, _icon);
		}
		inline void load(HINSTANCE const hInst, LPCWSTR const name, UINT const type, int const cx, int const cy, UINT const fuLoad) noexcept {
			_icon = static_cast<HICON>(LoadImageW(hInst, name, type, cx, cy, fuLoad));
		};
	private:
		HICON _icon = nullptr;
	};
}