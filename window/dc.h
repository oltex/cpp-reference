#pragma once
#include <Windows.h>

namespace window {
	class wnd;
	class object;
	class dc final {
		friend class bitmap;
	public:
		enum class flag : bool { get_dc, begin_paint };
	public:
		explicit dc(wnd const& wnd_, flag const flag_) noexcept;
		explicit dc(dc const& rhs) noexcept;
		~dc(void) noexcept;
	public:
		void select_object(object const& object_) noexcept;
		inline void pat_blt(int const x, int const y, int const w, int const h, DWORD const rop) const noexcept {
			PatBlt(_hdc, x, y, w, h, rop);
		}
		inline void bit_blt(int const x, int const y, int const cx, int const cy, dc const& dc_, int const x1, int const y1, DWORD const rop) const noexcept {
			BitBlt(_hdc, x, y, cx, cy, dc_._hdc, x1, y1, rop);
		}
		inline void move_to(int const x, int const y) const noexcept {
			MoveToEx(_hdc, x, y, nullptr);
		}
		inline void line_to(int const x, int const y) const noexcept {
			LineTo(_hdc, x, y);
		}
	private:
		wnd const* const _wnd = nullptr;
		HDC _hdc = nullptr;
		PAINTSTRUCT _ps;
	};
}

