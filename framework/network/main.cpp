#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "module/scheduler/io_complet_port.h"
#include "module/scheduler/thread_pool.h"
#include "server.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	io_complet_port::construct(16, 16);
	thread_pool::construct(16);
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
	thread_pool::destruct();
	io_complet_port::destruct();
	return 0;
}