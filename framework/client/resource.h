#pragma once

namespace framework {
	class resource {
	public:
		inline explicit resource(void) noexcept = default;
		inline explicit resource(resource const&) noexcept = delete;
		inline explicit resource(resource&&) noexcept = delete;
		inline auto operator=(resource const&) noexcept -> resource & = delete;
		inline auto operator=(resource&&) noexcept -> resource & = delete;
		inline virtual ~resource(void) noexcept;
	};
}