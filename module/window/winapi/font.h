#pragma once
#include "object.h"
#include <utility>

namespace winapi {
	class font final : public object {
	public:
		inline explicit font(int const height, int const width, int const escapement, int const orientation, int const weight, bool const italic, bool const under_line, bool strike_out,
			unsigned long const char_set, unsigned long const out_precision, unsigned long const clip_precision, unsigned long const quality, unsigned long const pitch_and_family, wchar_t const* const psz_face_name) noexcept
			: object(::CreateFontW(height, width, escapement, orientation, weight, italic, under_line, strike_out,
				char_set, out_precision, clip_precision, quality, pitch_and_family, psz_face_name)) {
		};
		inline explicit font(font const&) noexcept = delete;
		inline explicit font(font&& rhs) noexcept
			: object(std::move(rhs)) {
		};
		inline auto operator=(font const&) noexcept -> font & = delete;
		inline auto operator=(font&& rhs) noexcept -> font & = delete;
		inline virtual ~font(void) noexcept override = default;
	};
}