#pragma once
#include "object.h"
#include <utility>

namespace window {
	class bitmap final : public object {
	public:
		inline explicit bitmap(HBITMAP const hbitmap) noexcept
			: object(hbitmap) {
		}
		inline explicit bitmap(bitmap const&) noexcept = delete;
		inline explicit bitmap(bitmap&& rhs) noexcept
			: object(std::move(rhs)) {
		};
		inline auto operator=(bitmap const&) noexcept -> bitmap & = delete;
		inline auto operator=(bitmap&& rhs) noexcept -> bitmap & = delete;
		inline virtual ~bitmap(void) noexcept override = default;
	};
}