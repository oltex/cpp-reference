#include "performance_monitor.h"

int main(void) noexcept {
	setlocale(LC_ALL, "");
	performance_monitor& pm = performance_monitor::instance();
	std::wcout << L"안녕" << std::endl;

	pm.add_counter(L"process_time", L"\\Processor(_Total)\\% Processor Time");
	pm.collect_query();

	pm.test();
}

//* cpu total 사용률
//* cpu process 사용률
//* 프로세스 유저할당 메모리
//* 프로세스 논페이지 메모리
//* 사용가능 메모리
//* 논페이지드 메모리
//
//+ 네트워크 사용량