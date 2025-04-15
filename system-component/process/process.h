#pragma once
#include "../handle/handle.h"

namespace library::system {
	class token final : public handle {
	public:
		inline explicit token(void) noexcept = default;
		inline token(token const&) noexcept = default;
		inline explicit token(token&& rhs) noexcept
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
			if(0 == AdjustTokenPrivileges(_handle, false, &privileges, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr))
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
			if (0 == OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token_.data())) {
				switch (GetLastError()) {
				default:
					__debugbreak();
				}
			}
			return token_;
		}
	};
}



