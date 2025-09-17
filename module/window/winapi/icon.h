#pragma once
#include <Windows.h>

namespace winapi {
	class icon final {
		HICON _hicon;
		bool _share;
	public:
		inline explicit icon(HICON hicon) noexcept
			: _hicon(hicon), _share(true) {
		};
		inline explicit icon(icon const&) noexcept = delete;
		inline explicit icon(icon&&) noexcept = delete;
		inline auto operator=(icon const&) noexcept -> icon & = delete;
		inline auto operator=(icon&&) noexcept -> icon & = delete;
		inline ~icon(void) noexcept {
			if (nullptr != _hicon && false == _share)
				::DestroyIcon(_hicon);
		}

		inline auto draw(HDC const dc, int x, int y) noexcept {
			::DrawIcon(dc, x, y, _hicon);
		}
		inline auto data(void) const noexcept -> HICON {
			return _hicon;
		};
	};

	inline static auto load_icon(wchar_t const* const name) noexcept -> icon {
		return icon(::LoadIconW(nullptr, name));
	}
}