#include "library/debug.h"
#include "client.h"
#include <Windows.h>
#include <bitset>

int __stdcall wWinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE prev_hinstance, _In_ LPWSTR command_line, _In_ int command_show) {
	library::crt_set_debug_flag();
	library::component_initialize(COINIT_MULTITHREADED);
	{
		framework::client _client;
		_client.execute();
	}
	library::component_uninitialize();
	return 0;
}