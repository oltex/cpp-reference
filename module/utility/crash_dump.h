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

		//std::set_terminate() //설명: 프로그램에서 예외가 잡히지 않았을 때 호출되는 종료 처리기를 설정합니다. 사용: terminate() 함수 호출 시 실행될 핸들러를 등록.결론:작동안함
		//std::set_new_handler() //설명: operator new가 메모리 할당에 실패했을 때 호출될 핸들러를 설정합니다. 사용: 메모리 부족 시 특정 처리를 실행.
		//_set_abort_behavior() //설명: abort 호출 시 동작을 제어합니다. 사용 : 런타임에서 디버그 메시지 출력 여부를 설정. 결론:에러로 안잡아줌 어떻게 설정해도
		//_set_se_translator() //설명: 구조화된 예외를 C++ 예외로 변환하는 핸들러를 설정합니다. 사용 : Windows 환경에서 SEH를 표준 C++ 예외로 처리. 결론:에러가 아니다 예외처리 방식이라 이때 덤프를 낼 이유는 없음

		_CrtSetReportMode(_CRT_WARN, 0);
		_CrtSetReportMode(_CRT_ERROR, 0);
		_CrtSetReportMode(_CRT_ASSERT, 0);
		_CrtSetReportHook([](int, char*, int*) noexcept -> int { __debugbreak(); return 0; });
		//_CrtSetReportHook2();
		//_CrtSetReportHookW2();

		SetUnhandledExceptionFilter(detail::set_unhandled_exception_filter);
	}
}