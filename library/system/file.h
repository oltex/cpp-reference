#pragma once
#include "handle.h"
#include "component.h"
#include <string_view>
#include <Windows.h>
#include <shobjidl.h>

namespace library {
	class file : public handle {
	public:
		inline explicit file(void) noexcept = default;
		inline explicit file(std::wstring_view path, unsigned long desired_access, unsigned long share_mode, unsigned long creation_disposition, unsigned long flags_and_attributes) noexcept
			: handle(::CreateFileW(path.data(), desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, nullptr)) {
		};
		inline explicit file(file const&) noexcept = delete;
		inline explicit file(file&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(file const&) noexcept -> file & = delete;
		inline auto operator=(file&& rhs) noexcept -> file& {
			handle::operator=(std::move(rhs));
			return *this;
		};
		inline virtual ~file(void) noexcept override = default;

		inline void create(std::wstring_view path, unsigned long desired_access, unsigned long share_mode, unsigned long  creation_disposition, unsigned long flags_and_attributes) noexcept {
			_handle = ::CreateFileW(path.data(), desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, nullptr);
		}
		inline bool read(void* const buffer, unsigned long const length) const noexcept {
			return ::ReadFile(_handle, buffer, length, nullptr, nullptr);
		}
		inline bool write(void const* const buffer, unsigned long const length) const noexcept {
			return ::WriteFile(_handle, buffer, length, nullptr, nullptr);
		}
		inline auto set_pointer(long const distance_to_move, unsigned long move_method) const noexcept -> unsigned long {
			return ::SetFilePointer(_handle, distance_to_move, nullptr, move_method);
		}
		inline auto set_pointer_ex(LARGE_INTEGER const distance_to_move, unsigned long move_method) const noexcept -> bool {
			return ::SetFilePointerEx(_handle, distance_to_move, nullptr, move_method);
		}
		inline void set_end(void) const noexcept {
			::SetEndOfFile(_handle);
		}
		inline void flush_buffer(void) const noexcept {
			::FlushFileBuffers(_handle);
		}
		inline auto get_size_ex(void) const noexcept {
			LARGE_INTEGER size;
			::GetFileSizeEx(_handle, &size);
			return size;
		}
		inline bool set_file_completion_notification_mode(unsigned char flag) noexcept {
			return ::SetFileCompletionNotificationModes(_handle, flag);
		}
		inline static auto get_attribute(std::wstring_view const path) noexcept -> unsigned long {
			return ::GetFileAttributesW(path.data());
		}
		inline static bool create_directory(std::wstring_view const path) noexcept {
			return ::CreateDirectoryW(path.data(), nullptr);
		}
	};

	class shell_item : public component<IShellItem> {
		using base = component<IShellItem>;
		using size_type = unsigned int;
	public:
		inline explicit shell_item(IShellItem* component) noexcept
			: base(component) {
		};
		inline explicit shell_item(shell_item const&) noexcept = delete;
		inline explicit shell_item(shell_item&&) noexcept = delete;
		inline auto operator=(shell_item const&) noexcept -> shell_item & = delete;
		inline auto operator=(shell_item&& rhs) noexcept -> shell_item & = delete;
		inline ~shell_item(void) noexcept = default;

		inline auto get_display_name(SIGDN sigdn) noexcept -> PWSTR {
			PWSTR name;
			auto result = _component->GetDisplayName(sigdn, &name);
			assert(SUCCEEDED(result));
			return name;
		}
	};

	class file_open_dialog : public component<IFileOpenDialog> {
		using base = component<IFileOpenDialog>;
		using size_type = unsigned int;
	public:
		inline explicit file_open_dialog(void) noexcept
			: base(component_create_instance<IFileOpenDialog>(CLSID_FileOpenDialog)) {
		};
		inline explicit file_open_dialog(file_open_dialog const&) noexcept = delete;
		inline explicit file_open_dialog(file_open_dialog&&) noexcept = delete;
		inline auto operator=(file_open_dialog const&) noexcept -> file_open_dialog & = delete;
		inline auto operator=(file_open_dialog&& rhs) noexcept -> file_open_dialog & = delete;
		inline ~file_open_dialog(void) noexcept = default;

		template <size_type size>
		inline auto set_file_type(COMDLG_FILTERSPEC(&filter)[size]) noexcept {
			auto result = _component->SetFileTypes(size, filter);
			assert(SUCCEEDED(result));
		}
		inline auto set_file_type_index(size_type index) noexcept {
			auto result = _component->SetFileTypeIndex(1);
			assert(SUCCEEDED(result));
		}
		inline auto set_default_externsion(wchar_t const* const exten) noexcept {
			auto result = _component->SetDefaultExtension(exten);
			assert(SUCCEEDED(result));
		}
		inline bool show(HWND hwnd) noexcept {
			return SUCCEEDED(_component->Show(hwnd));
		}
		inline auto get_result(void) noexcept -> shell_item {
			IShellItem* item;
			auto result = _component->GetResult(&item);
			return shell_item(item);
		}
	};
}