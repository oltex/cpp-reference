#pragma once
#include "library/pattern/singleton.h"
#include "library/memory.h"
#include <Windows.h>

namespace window {
	inline static auto load_image(wchar_t const* const name, unsigned int const type, int const cx, int const cy, unsigned int const fu_load) noexcept -> HANDLE {
		return ::LoadImageW(nullptr, name, type, cy, cy, fu_load);
	}
	inline static auto load_cursor(wchar_t const* const name) noexcept -> HCURSOR {
		return ::LoadCursorW(nullptr, name);
	}
	inline static auto load_icon(wchar_t const* const name) noexcept -> HICON {
		return ::LoadIconW(nullptr, name);
	}

	class instance final {
		inline static HINSTANCE _hinstance = nullptr;
	public:
		inline explicit instance(void) noexcept = delete;
		inline explicit instance(instance const&) noexcept = delete;
		inline explicit instance(instance&&) noexcept = delete;
		inline auto operator=(instance const&) noexcept -> instance & = delete;
		inline auto operator=(instance&&) noexcept -> instance & = delete;
		inline ~instance(void) noexcept = delete;

		inline static void construct(HINSTANCE const hinstance) noexcept {
			_hinstance = hinstance;
		}
		inline static void destruct(void) noexcept {
		}

		inline static auto load_image(wchar_t const* const name, unsigned int const type, int const cx, int const cy, unsigned int const fu_load) noexcept -> HANDLE {
			return ::LoadImageW(_hinstance, name, type, cy, cy, fu_load);
		}
		inline static auto data(void) noexcept -> HINSTANCE {
			return _hinstance;
		}
	};
}

//(static_cast<HBITMAP>(LoadImageW(hInst, name, IMAGE_BITMAP, cx, cy, fuLoad)))