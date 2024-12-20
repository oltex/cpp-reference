#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "window_socket_api/initialize.h"
#include "socket.h"
#include "socket_address.h"
#include "domain.h"
#include "select.h"
#include "file_descriptor.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//network::window_socket_api::start_up();

	////network::domain domain;
	////list<network::storage> storage = domain.get_storage(L"www.naver.com", nullptr);
	////for (auto& iter : storage)
	////	std::cout << iter.get_address() << std::endl;

	//network::socket socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//socket.set_linger(1, 0);
	//socket.set_nonblocking(1);
	//socket.set_tcp_nodelay(1);

	//network::socket_address_ipv4 socket_address;
	//socket_address.set_address("192.168.10.79");
	//socket_address.set_port(20000);
	////socket.bind(reinterpret_cast<network::storage&>(storage));
	//socket.connect(socket_address);

	//char buf[] = "hello!";
	//socket.send_to(buf, 6, 0, socket_address);

	////network::storage storage2;
	////int length = 999;
	////char buf2[7] = "";
	////socket.receive_from(buf2, 6, 0, storage2, length);

	//system("pause");
	//socket.set_tcp_nodelay(true);

	//network::window_socket_api::clean_up();
	return 0;
}