#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "iocp.h"
#include "server.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto& iocp = framework::iocp::instance();
	iocp.create(16, 16);
	//-------------------------
	library::wsa_start_up();
	library::socket::wsa_io_control_acccept_ex();
	library::socket::wsa_io_control_connect_ex();
	library::socket::wsa_io_control_get_accept_ex_sockaddr();

	framework::server server;
	server.start();
	server.accept("127.0.0.1", 6000, 65535);
	system("pause");
	//server.reject();
	//-------------------------
	system("pause");
	//-------------------------
	server.stop();


	library::wsa_clean_up();
	//-------------------------
	iocp.close();
	return 0;
}