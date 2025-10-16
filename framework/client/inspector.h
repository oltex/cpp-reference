#pragma once
#include "editor.h"

namespace framework {
	class inspector : public editor {
	public:
		explicit inspector(void) noexcept = default;
		explicit inspector(inspector const&) noexcept = delete;
		explicit inspector(inspector&&) noexcept = delete;
		auto operator=(inspector const&) noexcept -> inspector & = delete;
		auto operator=(inspector&&) noexcept -> inspector & = delete;
		virtual ~inspector(void) noexcept override = default;

		virtual void update(void) noexcept override;
	};
}