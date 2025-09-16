#pragma once

namespace framework {
	class system {
		inline explicit system(void) noexcept = default;
		inline explicit system(system const&) noexcept = delete;
		inline explicit system(system&&) noexcept = delete;
		inline auto operator=(system const&) noexcept -> system & = delete;
		inline auto operator=(system&&) noexcept -> system & = delete;
		inline ~system(void) noexcept = default;
	};
}