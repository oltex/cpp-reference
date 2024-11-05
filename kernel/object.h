#pragma once
#include <Windows.h>

namespace kernel {
	class object {
	public:
		inline explicit object(void) noexcept
			: _handle(INVALID_HANDLE_VALUE) {
		};
		inline explicit object(HANDLE const handle) noexcept
			: _handle(handle) {
		};
		inline explicit object(object const& rhs) noexcept = delete;
		inline explicit object(object&& rhs) noexcept
			: _handle(rhs._handle) {
			rhs._handle = INVALID_HANDLE_VALUE;
		};
		inline auto operator=(object const& rhs) noexcept -> object & = delete;
		inline auto operator=(object&& rhs) noexcept -> object& {
			CloseHandle(_handle);
			_handle = rhs._handle;
			rhs._handle = INVALID_HANDLE_VALUE;
			return *this;
		};
		inline virtual ~object(void) noexcept {
			CloseHandle(_handle);
		};
	public:
		inline void close(void) noexcept {
			CloseHandle(_handle);
			_handle = INVALID_HANDLE_VALUE;
		}
	public:
		inline auto data(void) noexcept -> HANDLE& {
			return _handle;
		}
	protected:
		HANDLE _handle;
	};
}