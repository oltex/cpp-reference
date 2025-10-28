#pragma once
#include "library/json.hpp"

namespace framework {
	class level {
	public:
		explicit level(void) noexcept = default;
		explicit level(nlohmann::json const& json) noexcept;
		explicit level(level const&) noexcept = delete;
		explicit level(level&&) noexcept = delete;
		auto operator=(level const&) noexcept -> level & = delete;
		auto operator=(level&&) noexcept -> level & = delete;
		~level(void) noexcept = default;
	};
}