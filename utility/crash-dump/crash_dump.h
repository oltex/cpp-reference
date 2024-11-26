#pragma once

#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <iostream>
#include <intrin.h>
#include <new.h>
#include <eh.h>

class crash_dump final {
public:
	inline explicit crash_dump(void) noexcept {
		_set_invalid_parameter_handler([](const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t) -> void { __debugbreak(); });
		//_set_thread_local_invalid_parameter_handler
		_set_purecall_handler([](void) noexcept -> void { __debugbreak(); });

		//std::set_terminate() //����: ���α׷����� ���ܰ� ������ �ʾ��� �� ȣ��Ǵ� ���� ó���⸦ �����մϴ�. ���: terminate() �Լ� ȣ�� �� ����� �ڵ鷯�� ���.
		//std::set_new_handler() //����: operator new�� �޸� �Ҵ翡 �������� �� ȣ��� �ڵ鷯�� �����մϴ�. ���: �޸� ���� �� Ư�� ó���� ����.
		//_set_abort_behavior() //����: abort ȣ�� �� ������ �����մϴ�. ��� : ��Ÿ�ӿ��� ����� �޽��� ��� ���θ� ����. ���:������ ������� ��� �����ص�
		//_set_se_translator() //����: ����ȭ�� ���ܸ� C++ ���ܷ� ��ȯ�ϴ� �ڵ鷯�� �����մϴ�. ��� : Windows ȯ�濡�� SEH�� ǥ�� C++ ���ܷ� ó��. ���:������ �ƴϴ� ����ó�� ����̶� �̶� ������ �� ������ ����
	
		//_CrtSetReportMode(_CRT_WARN, 0);
		//_CrtSetReportMode(_CRT_ERROR, 0);
		//_CrtSetReportMode(_CRT_ASSERT, 0);
		//_CrtSetReportHook();
		//_CrtSetReportHook2();
		//_CrtSetReportHookW2();

		SetUnhandledExceptionFilter(test);
	};
	inline explicit crash_dump(crash_dump const& rhs) noexcept = delete;
	inline explicit crash_dump(crash_dump&& rhs) noexcept = delete;
	inline auto operator=(crash_dump const& rhs) noexcept -> crash_dump & = delete;
	inline auto operator=(crash_dump&& rhs) noexcept -> crash_dump & = delete;
	inline ~crash_dump(void) noexcept = default;
public:
	inline static auto _stdcall test(__in EXCEPTION_POINTERS* exception_pointer) noexcept -> LONG {
		CreateFileW(L"test.txt", FILE_WRITE_DATA, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
		return 0;
	}
	//inline static void invalid_parameter_handler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t) noexcept {
	//	__debugbreak();
	//}
	//inline static void purecall_handler(void) noexcept {
	//	__debugbreak();
	//}
	
};