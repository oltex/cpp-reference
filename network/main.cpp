#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "rpc.h"
#include "server.h"

#include "network.h"
#include "socket.h"
#include "domain.h"
#include "poll.h"

#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	network::start_up();

	//network::domain domain;
	//list<network::storage> storage = domain.get_storage(L"www.naver.com", nullptr);
	//for (auto& iter : storage)
	//	std::cout << iter.get_address() << std::endl;

	network::socket socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//socket.set_linger(1, 0);
	socket.set_nonblocking(1);
	//socket.set_tcp_nodelay(1);

	network::storage_ipv4 storage;
	storage.set_address("192.168.10.79");
	storage.set_port(20000);
	//socket.bind(reinterpret_cast<network::storage&>(storage));
	socket.connect(reinterpret_cast<network::storage&>(storage));

	char buf[] = "hello!";
	socket.send_to(buf, 6, 0, reinterpret_cast<network::storage&>(storage));

	network::storage storage2;
	int length = 999;
	char buf2[7] = "";
	socket.receive_from(buf2, 6, 0, storage2, length);

	system("pause");
	socket.set_tcp_nodelay(true);

	server<session, remote_procedure_call> a;

	network::clean_up();
	return 0;
}