#pragma once
#include <Windows.h>

namespace thread {
	class mutex final {
	public:
		inline explicit mutex(bool const initial_owner) noexcept
			: _mutex(CreateMutexW(nullptr, initial_owner, nullptr)) {
		};
		inline explicit mutex(mutex const& rhs) noexcept = delete;
		inline explicit mutex(mutex&& rhs) noexcept = delete;
		inline auto operator=(mutex const& rhs) noexcept -> mutex & = delete;
		inline auto operator=(mutex&& rhs) noexcept -> mutex & = delete;
		inline ~mutex(void) noexcept {
			CloseHandle(_mutex);
		};
	public:

	private:
		HANDLE _mutex;
	};
}