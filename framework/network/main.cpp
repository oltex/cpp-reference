#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "iocp.h"
#include "server.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	framework::iocp::construct(16, 16);
	//-------------------------
	library::wsa_start_up();
	library::socket::wsa_io_control_acccept_ex();
	library::socket::wsa_io_control_connect_ex();
	library::socket::wsa_io_control_get_accept_ex_sockaddr();
	//-------------------------
	{
		framework::server server(100);
		server.start_listen("127.0.0.1", 6000, 65535);
		system("pause");
		server.stop_listen();
	}
	//-------------------------
	library::wsa_clean_up();
	//-------------------------
	framework::iocp::destruct();
	return 0;
}