#pragma once
#include "library/system/interlock.h"
#include "library/system/file.h"
#include "library/system/thread.h"
#include "library/system/process.h"
#include "library/system/time.h"
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
#include <sstream>

namespace detail {
	inline static auto _stdcall set_unhandled_exception_filter(EXCEPTION_POINTERS* exception_pointer) noexcept -> long {
		static unsigned int lock = 0;
		if (0 == library::interlock_exchange(lock, 1)) {
			auto time_t = library::time64();
			auto tm = library::local_time(time_t);

			std::wostringstream stream;
			stream << std::put_time(&tm, L"%y%m%d_%H%M%S") << L".dmp";

			library::file file(stream.str().data(), FILE_WRITE_DATA, FILE_SHARE_READ, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH);

			_MINIDUMP_EXCEPTION_INFORMATION infomation{
				.ThreadId = library::thread::get_current_id(),
				.ExceptionPointers = exception_pointer,
				.ClientPointers = true
			};
			MiniDumpWriteDump(library::process::get_current(), library::process::get_current_id(), file.data(), MiniDumpWithFullMemory, &infomation, nullptr, nullptr);
		}
		return 0;
	}
}

namespace library {
	inline void crash_dump(void) noexcept {
		_set_invalid_parameter_handler([](const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t) noexcept -> void { __debugbreak(); });
		//_set_thread_local_invalid_parameter_handler([](const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t) -> void { __debugbreak(); });
		_set_purecall_handler([](void) noexcept -> void { __debugbreak(); });

		//std::set_terminate() //����: ���α׷����� ���ܰ� ������ �ʾ��� �� ȣ��Ǵ� ���� ó���⸦ �����մϴ�. ���: terminate() �Լ� ȣ�� �� ����� �ڵ鷯�� ���.���:�۵�����
		//std::set_new_handler() //����: operator new�� �޸� �Ҵ翡 �������� �� ȣ��� �ڵ鷯�� �����մϴ�. ���: �޸� ���� �� Ư�� ó���� ����.
		//_set_abort_behavior() //����: abort ȣ�� �� ������ �����մϴ�. ��� : ��Ÿ�ӿ��� ����� �޽��� ��� ���θ� ����. ���:������ ������� ��� �����ص�
		//_set_se_translator() //����: ����ȭ�� ���ܸ� C++ ���ܷ� ��ȯ�ϴ� �ڵ鷯�� �����մϴ�. ��� : Windows ȯ�濡�� SEH�� ǥ�� C++ ���ܷ� ó��. ���:������ �ƴϴ� ����ó�� ����̶� �̶� ������ �� ������ ����

		_CrtSetReportMode(_CRT_WARN, 0);
		_CrtSetReportMode(_CRT_ERROR, 0);
		_CrtSetReportMode(_CRT_ASSERT, 0);
		_CrtSetReportHook([](int, char*, int*) noexcept -> int { __debugbreak(); return 0; });
		//_CrtSetReportHook2();
		//_CrtSetReportHookW2();

		SetUnhandledExceptionFilter(detail::set_unhandled_exception_filter);
	}
}