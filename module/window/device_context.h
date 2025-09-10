#pragma once
#include "library/function.h"
#include "object.h"
#include "bitmap.h"
#include "brush.h"
#include <Windows.h>

namespace window {
	class device_context final {
		HDC _hdc;
	public:
		inline explicit device_context(HDC const hdc) noexcept
			: _hdc(hdc) {
		}
		inline explicit device_context(device_context const& rhs) noexcept = delete;
		inline explicit device_context(device_context&& rhs) noexcept
			: _hdc(library::exchange(rhs._hdc, nullptr)) {
		};
		inline auto operator=(device_context const&) noexcept -> device_context & = delete;
		inline auto operator=(device_context&&) noexcept -> device_context & = delete;
		inline ~device_context(void) noexcept {
			if (nullptr != _hdc)
				DeleteDC(_hdc);
		};

		inline auto create_compatible_device_context(void) const noexcept -> device_context {
			HDC hdc = ::CreateCompatibleDC(_hdc);
			return device_context(hdc);
		}
		inline auto create_compatible_bitmap(int const cx, int const cy) const noexcept {
			HBITMAP hbitmap = ::CreateCompatibleBitmap(_hdc, cx, cy);
			return bitmap(hbitmap);
		}
	public:
		inline void select_object(object& object_) noexcept {
			SelectObject(_hdc, object_.data());
		}
		inline void select_object(HGDIOBJ const& hgdiobj) noexcept {
			SelectObject(_hdc, hgdiobj);
		}
		inline void fill_rect(RECT const* const rect, brush& brush_) noexcept {
			FillRect(_hdc, rect, (HBRUSH)brush_.data());
		}
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
		inline auto ellipse(int const left, int const top, int const right, int const bottom) const noexcept -> bool {
			return ::Ellipse(_hdc, left, top, right, bottom);
		}
		inline void pat_blt(int const x, int const y, int const w, int const h, DWORD const rop) const noexcept {
			::PatBlt(_hdc, x, y, w, h, rop);
		}
		inline void bit_blt(int const x, int const y, int const cx, int const cy, device_context const& dc, int const x1, int const y1, DWORD const rop) const noexcept {
			::BitBlt(_hdc, x, y, cx, cy, dc._hdc, x1, y1, rop);
		}
		inline auto move_to(int const x, int const y) const noexcept -> bool {
			return ::MoveToEx(_hdc, x, y, nullptr);
		}
		inline auto line_to(int const x, int const y) const noexcept -> bool {
			return ::LineTo(_hdc, x, y);
		}
		inline auto rectangle(int const left, int const top, int const right, int const bottom) const noexcept -> bool {
			return ::Rectangle(_hdc, left, top, right, bottom);
		}

		inline auto data(void) noexcept -> HDC& {
			return _hdc;
		}
	};
}