#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
#include <iostream>
#include <intrin.h>
#include <new.h>
#include <eh.h>
#include <chrono>
#include <string>

#include "logger.h"

class crash_dump final {
public:
	inline explicit crash_dump(void) noexcept {
		_set_invalid_parameter_handler([](const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t) noexcept -> void { __debugbreak(); });
		_set_purecall_handler([](void) noexcept -> void { __debugbreak(); });

		_CrtSetReportMode(_CRT_WARN, 0);
		_CrtSetReportMode(_CRT_ERROR, 0);
		_CrtSetReportMode(_CRT_ASSERT, 0);
		_CrtSetReportHook([](int, char*, int*) noexcept -> int { __debugbreak(); return 0; });

		SetUnhandledExceptionFilter(unhandled_exception_filter);
	};
	inline explicit crash_dump(crash_dump const& rhs) noexcept = delete;
	inline explicit crash_dump(crash_dump&& rhs) noexcept = delete;
	inline auto operator=(crash_dump const& rhs) noexcept -> crash_dump & = delete;
	inline auto operator=(crash_dump&& rhs) noexcept -> crash_dump & = delete;
	inline ~crash_dump(void) noexcept = default;
public:
	inline static auto _stdcall unhandled_exception_filter(__in EXCEPTION_POINTERS* exception_pointer) noexcept -> long {
		volatile static unsigned int _lock = 0;
		if (0 == _InterlockedExchange(&_lock, 1)) {
			HANDLE file = CreateFileW(L"dump.txt", FILE_WRITE_DATA, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);

			_MINIDUMP_EXCEPTION_INFORMATION minidump_exception_information;
			minidump_exception_information.ThreadId = GetCurrentThreadId();
			minidump_exception_information.ExceptionPointers = exception_pointer;
			minidump_exception_information.ClientPointers = true;
			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpWithFullMemory, &minidump_exception_information, nullptr, nullptr);

			CloseHandle(file);

			HANDLE file = CreateFileW(L"log.txt", FILE_WRITE_DATA, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
		}
		return 0;
	}
};