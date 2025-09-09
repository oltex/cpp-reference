#pragma once
#include "handle.h"

namespace library {
	class mutex final : public handle {
	public:
		inline explicit mutex(bool const initial_owner) noexcept
			: handle(::CreateMutexW(nullptr, initial_owner, nullptr)) {
		};
		inline explicit mutex(mutex const& rhs) noexcept = delete;
		inline explicit mutex(mutex&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(mutex const& rhs) noexcept -> mutex & = delete;
		inline auto operator=(mutex&& rhs) noexcept -> mutex& {
			handle::operator=(std::move(rhs));
		};
		inline virtual ~mutex(void) noexcept override = default;
	};
}