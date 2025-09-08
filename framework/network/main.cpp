#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "module/scheduler/io_complet_port.h"
#include "module/utility/crash_dump.h"
#include "server.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	library::crash_dump();
	library::wsa_start_up();
	library::socket::wsa_io_control_acccept_ex();
	library::socket::wsa_io_control_connect_ex();
	library::socket::wsa_io_control_get_accept_ex_sockaddr();

	io_complet_port::construct(8, 8);
	//-------------------------
	{
		framework::server server(8, 100);
		server.start_listen("127.0.0.1", 6000, 65535);
		system("pause");
		server.stop_listen();
		system("pause");
	}
	//-------------------------
	io_complet_port::destruct();

	library::wsa_clean_up();

	return 0;
}