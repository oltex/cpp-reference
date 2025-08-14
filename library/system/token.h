#pragma once
#include "handle.h"
#include <process.h>

namespace library {
	class token final : public handle {
	public:
		inline explicit token(void) noexcept = default;
		inline explicit token(token const&) noexcept = delete;
		inline token(token&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(token const&) noexcept -> token & = delete;
		inline auto operator=(token&& rhs) noexcept -> token& {
			handle::operator=(std::move(rhs));
			return *this;
		};
		inline virtual ~token(void) noexcept override = default;

		inline void adjust_privileges_se_lock_memory_privilege(void) noexcept {
			LUID luid;
			if (0 == LookupPrivilegeValueW(nullptr, L"SeLockMemoryPrivilege", &luid)) {
				switch (GetLastError()) {
				default:
					__debugbreak();
				}
			}

			TOKEN_PRIVILEGES privileges;
			privileges.PrivilegeCount = 1;
			privileges.Privileges[0].Luid = luid;
			privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (0 == AdjustTokenPrivileges(_handle, false, &privileges, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr))
				__debugbreak();
			else {
				switch (GetLastError()) {
				case ERROR_SUCCESS:
					break;
				case ERROR_NOT_ALL_ASSIGNED:
				default:
					__debugbreak();
				}
			}
		}
	};
}