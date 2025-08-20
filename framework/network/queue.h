#pragma once
#include "library/container/lockfree/queue.h"
#include "message.h"

namespace framework {
	class queue final : public library::lockfree::queue<message, false> {
	public:
		inline explicit queue(void) noexcept = default;
		inline explicit queue(queue const&) noexcept = delete;
		inline explicit queue(queue&&) noexcept = delete;
		inline auto operator=(queue const&) noexcept -> queue & = delete;
		inline auto operator=(queue&&) noexcept -> queue & = delete;
		inline ~queue(void) noexcept = default;
	};
}