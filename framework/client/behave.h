#pragma once
#include "system.h"

namespace framework {
	class behave : public system {
	public:
		explicit behave(void) noexcept = default;
		explicit behave(behave const&) noexcept = delete;
		explicit behave(behave&&) noexcept = delete;
		auto operator=(behave const&) noexcept -> behave & = delete;
		auto operator=(behave&&) noexcept -> behave & = delete;
		virtual ~behave(void) noexcept override = default;

		virtual void update(void) noexcept override;
	};
}