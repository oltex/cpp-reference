#include "performance_monitor.h"

int main(void) noexcept {
	utility::performance_monitor& pm = utility::performance_monitor::instance();
	auto objects = pm.enum_object();
	auto items = pm.enum_item_instance(L"Processor");
	std::wcout << "=================================================" << std::endl;
	std::wcout << "/// object ///" << std::endl;
	for (auto& iter : objects)
		std::wcout << iter << std::endl;
	std::wcout << "/// item ///" << std::endl;
	for (auto& iter : items.first)
		std::wcout << iter << std::endl;
	std::wcout << "/// instance ///" << std::endl;
	for (auto& iter : items.second)
		std::wcout << iter << std::endl;
	std::wcout << "=================================================" << std::endl;


	pm.add_counter(L"processor_time_total", L"Processor", L"Processor Time", L"_Total");
	pm.add_counter(L"processor_time_all", L"\\Processor(*)\\% Processor Time");
	for (;;) {
		pm.collect_query_data();
		auto result = pm.get_formatted_counter_value(L"processor_time_total", PDH_FMT_DOUBLE);
		auto [size, list] = pm.get_formatted_counter_array(L"processor_time_all", PDH_FMT_DOUBLE);

		std::wcout << "=================================================" << std::endl;
		std::wcout << "processor_time_total :" << result.doubleValue << std::endl;
		std::wcout << "processor_time_all :" << std::endl;
		for (int index = 0; index < size; ++index)
			std::wcout << list[index].szName << L" : " << list[index].FmtValue.doubleValue << std::endl;
		std::wcout << "=================================================" << std::endl;

		Sleep(1000);
	}
}

//pm.test();

//* cpu total 사용률
//* cpu process 사용률
//* 프로세스 유저할당 메모리
//* 프로세스 논페이지 메모리
//* 사용가능 메모리
//* 논페이지드 메모리
//+ 네트워크 사용량