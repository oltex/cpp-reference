#pragma once
#include "object.h"
#include <utility>
#include <Windows.h>

namespace window {
	class brush final : public object {
	public:
		inline explicit brush(COLORREF const color) noexcept
			: object(CreateSolidBrush(color)) {
		};
		inline explicit brush(brush const&) noexcept = delete;
		inline explicit brush(brush&& rhs) noexcept
			: object(std::move(rhs)) {
		};
		inline auto operator=(brush const&) noexcept -> brush & = delete;
		inline auto operator=(brush&& rhs) noexcept -> brush & = delete;
		inline virtual ~brush(void) noexcept override = default;
	};
}