#pragma once
#include "resource.h"

namespace framework {
	class mesh : public resource {
	public:
		explicit mesh(void) noexcept;
		explicit mesh(mesh const& rhs) noexcept = delete;
		explicit mesh(mesh&&) noexcept = delete;
		auto operator=(mesh const&) noexcept -> mesh & = delete;
		auto operator=(mesh&&) noexcept -> mesh & = delete;
		virtual ~mesh(void) noexcept override;
	};
}