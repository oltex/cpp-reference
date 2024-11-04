#pragma once
#include "../kernel/object.h"
#include <string_view>
#include <Windows.h>

class file final : public kernel::object {
public:
	inline explicit file(void) noexcept = default;
	inline explicit file(wchar_t const* const path, unsigned long desired_access, unsigned long share_mode, unsigned long creation_disposition, unsigned long flags_and_attributes) noexcept 
		: object(CreateFileW(path, desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, nullptr)) {
	};
	inline explicit file(file const& rhs) noexcept = delete;
	inline explicit file(file&& rhs) noexcept = delete;
	inline auto operator=(file const& rhs) noexcept -> file & = delete;
	inline auto operator=(file&& rhs) noexcept -> file & = delete;
	inline virtual ~file(void) noexcept override = default;
public:
	inline void create(wchar_t const* const path, unsigned long desired_access, unsigned long share_mode, unsigned long  creation_disposition, unsigned long flags_and_attributes) noexcept {
		CloseHandle(_handle);
		_handle = CreateFileW(path, desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, nullptr);
	}
	inline void close(void) noexcept {
		CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
	inline void read(void* const buffer, unsigned long const length) const noexcept {
		ReadFile(_handle, buffer, length, nullptr, nullptr);
	}
	inline void write(void const* const buffer, unsigned long const length) const noexcept {
		WriteFile(_handle, buffer, length, nullptr, nullptr);
	}
public:
	inline auto set_pointer(long const distance_to_move, unsigned long move_method) const noexcept -> unsigned long {
		return SetFilePointer(_handle, distance_to_move, nullptr, move_method);
	}
	inline void set_end(void) const noexcept {
		SetEndOfFile(_handle);
	}
	inline void flush_buffer(void) const noexcept {
		FlushFileBuffers(_handle);
	}
};