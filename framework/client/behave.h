#pragma once
#include "system.h"
#include "behaviour.h"

namespace framework {
	class behave : system<behaviour> {
	public:
		inline explicit behave(void) noexcept = default;
		inline explicit behave(behave const&) noexcept = delete;
		inline explicit behave(behave&&) noexcept = delete;
		inline auto operator=(behave const&) noexcept -> behave & = delete;
		inline auto operator=(behave&&) noexcept -> behave & = delete;
		inline behave(void) noexcept = default;

		inline virtual void update(void) noexcept override {
			for (auto& component : _component)
				component.get<0>()->update();
		}
	};
}