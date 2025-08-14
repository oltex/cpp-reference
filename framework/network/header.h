#pragma once

namespace framework {
	struct header final {
		unsigned short _size;
		inline explicit header(void) noexcept = default;
		inline explicit header(header const&) noexcept = delete;
		inline explicit header(header&&) noexcept = delete;
		inline auto operator=(header const&) noexcept -> header & = delete;
		inline auto operator=(header&&) noexcept -> header & = delete;
		inline ~header(void) noexcept = default;
	};
}