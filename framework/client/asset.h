#pragma once
#include "editor.h"

namespace framework {
	class asset : public editor {
	public:
		explicit asset(void) noexcept = default;
		explicit asset(asset const&) noexcept = delete;
		explicit asset(asset&&) noexcept = delete;
		auto operator=(asset const&) noexcept -> asset & = delete;
		auto operator=(asset&&) noexcept -> asset & = delete;
		virtual ~asset(void) noexcept override = default;

		virtual void update(void) noexcept override;

		void import(void) noexcept;
		void search(void) noexcept;
	};
}