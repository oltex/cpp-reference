#include "performance_monitor.h"

int main(void) noexcept {
	setlocale(LC_ALL, "");
	performance_monitor& pm = performance_monitor::instance();
	std::wcout << L"�ȳ�" << std::endl;

	pm.add_counter(L"process_time", L"\\Processor(_Total)\\% Processor Time");
	pm.collect_query();

	pm.test();
}

//* cpu total ����
//* cpu process ����
//* ���μ��� �����Ҵ� �޸�
//* ���μ��� �������� �޸�
//* ��밡�� �޸�
//* ���������� �޸�
//
//+ ��Ʈ��ũ ��뷮