#include "library/debug.h"
#include "client.h"
#include <Windows.h>

int __stdcall wWinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE prev_hinstance, _In_ LPWSTR command_line, _In_ int command_show) {
	library::crt_set_debug_flag();
	{
		framework::client _client;
		_client.execute();
	}
	//return static_cast<int>(msg.wParam);
	return 0;
}