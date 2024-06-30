#pragma once
#include <Windows.h>

namespace window {
	class wnd;
	class object;
	class brush;
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
		void fill_rect(RECT const* const rect, window::brush const& brush_) noexcept;
	public:
		inline void set_bk_mode(int const mode) const noexcept {
			SetBkMode(_hdc, mode);
		}
		inline void set_text_color(COLORREF const color) const noexcept {
			SetTextColor(_hdc, color);
		}
		inline void draw_text(LPCWSTR const lpchText, int const cchText, LPRECT const lprc, UINT const format) const noexcept {
			DrawTextW(_hdc, lpchText, cchText, lprc, format);
		}
		inline void text_out(int const x, int const y, LPCWSTR const lpString, int const c) const noexcept {
			TextOutW(_hdc, x, y, lpString, c);
		}
		inline void ellipse(int const left, int const top, int const right, int const bottom) const noexcept {
			Ellipse(_hdc, left, top, right, bottom);
		}
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
		inline void rectangle(int const left, int const top, int const right, int const bottom) const noexcept {
			Rectangle(_hdc, left, top, right, bottom);
		}
	private:
		wnd const* const _wnd = nullptr;
		HDC _hdc = nullptr;
		PAINTSTRUCT _ps;
	};
}

