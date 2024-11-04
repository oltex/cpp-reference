#pragma once
#include "../kernel/object.h"
#include <Windows.h>

namespace multi {
	class mutex final : public kernel::object {
	public:
		inline explicit mutex(bool const initial_owner) noexcept
			: object(CreateMutexW(nullptr, initial_owner, nullptr)) {
		};
		inline explicit mutex(mutex const& rhs) noexcept = delete;
		inline explicit mutex(mutex&& rhs) noexcept = delete;
		inline auto operator=(mutex const& rhs) noexcept -> mutex & = delete;
		inline auto operator=(mutex&& rhs) noexcept -> mutex & = delete;
		inline virtual ~mutex(void) noexcept override = default;
	public:
	};
}