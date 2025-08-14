#pragma once
#include "handle.h"
#include "token.h"
#include <process.h>

namespace library {
	class process final : public handle {
	public:
		inline explicit process(void) noexcept
			: handle(GetCurrentProcess()) {
		};
		inline explicit process(process const&) noexcept = delete;
		inline explicit process(process&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(process const&) noexcept -> process & = delete;
		inline auto operator=(process&& rhs) noexcept -> process& {
			handle::operator=(std::move(rhs));
			return *this;
		};
		inline virtual ~process(void) noexcept override = default;

		inline auto open_token(void) noexcept -> token {
			token token_;
			if (0 == OpenProcessToken(_handle, TOKEN_ADJUST_PRIVILEGES, &token_.data())) {
				switch (GetLastError()) {
				default:
					__debugbreak();
				}
			}
			return token_;
		}
	};
}