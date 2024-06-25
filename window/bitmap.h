#pragma once
#include "object.h"

namespace window {
	class dc;
	class bitmap final : public object {
	public:
		explicit bitmap(dc const& dc_) noexcept;
		virtual ~bitmap(void) noexcept override = default;
	};
}