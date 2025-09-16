#pragma once
#include "component.h"

namespace framework {
	class behaviour : public component {
	public:
		inline explicit behaviour(void) noexcept = default;
		inline explicit behaviour(behaviour const&) noexcept = delete;
		inline explicit behaviour(behaviour&&) noexcept = delete;
		inline auto operator=(behaviour const&) noexcept -> behaviour & = delete;
		inline auto operator=(behaviour&&) noexcept -> behaviour & = delete;
		inline virtual ~behaviour(void) noexcept override = default;

		inline virtual void update(void) noexcept = 0;
	};
}