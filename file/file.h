#pragma once
#include <string_view>
#include <Windows.h>

class file final {
public:
	inline explicit file(void) noexcept = default;
	inline explicit file(wchar_t const* const path, unsigned long desired_access, unsigned long share_mode, unsigned long creation_disposition, unsigned long flags_and_attributes) noexcept {
		_file = CreateFileW(path, desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, nullptr);
	};
	inline explicit file(file const& rhs) noexcept = delete;
	inline explicit file(file&& rhs) noexcept
		: _file(rhs._file) {
		rhs._file = INVALID_HANDLE_VALUE;
	}
	inline auto operator=(file const& rhs) noexcept -> file & = delete;
	inline auto operator=(file&& rhs) noexcept -> file & = delete;
	inline ~file(void) noexcept {
		CloseHandle(_file);
	};
public:
	inline void create(wchar_t const* const path, unsigned long desired_access, unsigned long share_mode, unsigned long  creation_disposition, unsigned long flags_and_attributes) noexcept {
		CloseHandle(_file);
		_file = CreateFileW(path, desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, nullptr);
	}
	inline void close(void) noexcept {
		CloseHandle(_file);
		_file = INVALID_HANDLE_VALUE;
	}
	inline void read(void* const buffer, unsigned long const length) const noexcept {
		ReadFile(_file, buffer, length, nullptr, nullptr);
	}
	inline void write(void const* const buffer, unsigned long const length) const noexcept {
		WriteFile(_file, buffer, length, nullptr, nullptr);
	}
public:
	inline auto set_pointer(long const distance_to_move, unsigned long move_method) const noexcept -> unsigned long {
		return SetFilePointer(_file, distance_to_move, nullptr, move_method);
	}
	inline void set_end(void) const noexcept {
		SetEndOfFile(_file);
	}
	inline void flush_buffer(void) const noexcept {
		FlushFileBuffers(_file);
	}
private:
	HANDLE _file = INVALID_HANDLE_VALUE;
};