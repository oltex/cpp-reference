#pragma once
#include "object.h"
#include <utility>

namespace window {
	class bitmap final : public object {
	public:
		inline explicit bitmap(HBITMAP const hbitmap) noexcept
			: object(hbitmap) {
		}
		inline explicit bitmap(HINSTANCE const hInst, LPCWSTR const name, int const cx, int const cy, unsigned int const fuLoad) noexcept
			: object(static_cast<HBITMAP>(LoadImageW(hInst, name, IMAGE_BITMAP, cx, cy, fuLoad))) {
		}
		inline explicit bitmap(bitmap const& rhs) noexcept = delete;
		inline explicit bitmap(bitmap&& rhs) noexcept
			: object(std::move(rhs)) {
		};
		inline auto operator=(bitmap const& rhs) noexcept -> bitmap & = delete;
		inline auto operator=(bitmap&& rhs) noexcept -> bitmap & = delete;
		inline virtual ~bitmap(void) noexcept override = default;
	};
}