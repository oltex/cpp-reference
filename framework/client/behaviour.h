#pragma once
#include "component.h"

namespace framework {
	class behaviour : public component {
	public:
		explicit behaviour(size_type const type_id) noexcept;
		explicit behaviour(behaviour const&) noexcept = delete;
		explicit behaviour(behaviour&&) noexcept = delete;
		auto operator=(behaviour const&) noexcept -> behaviour & = delete;
		auto operator=(behaviour&&) noexcept -> behaviour & = delete;
		virtual ~behaviour(void) noexcept override = default;

		virtual void start(void) noexcept {};
		virtual void update(void) noexcept = 0;
	};
}