#pragma once
#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
//#include <WS2tcpip.h>

namespace network {
	//enum class address_family : int { inet = AF_INET, inet6 = AF_INET6 };
	//using enum address_family;
	//enum class type : int { stream = SOCK_STREAM, datagram = SOCK_DGRAM };
	//using enum type;
	//enum class protocol : int { tcp = IPPROTO_TCP, udp = IPPROTO_UDP };
	//using enum protocol;

	class network {
	public:
		inline explicit network(void) noexcept {
			WSAData wsadata;
			if (0 != WSAStartup(0x0202, &wsadata))
				DebugBreak();
		};
		inline ~network(void) noexcept {
			if (SOCKET_ERROR == WSACleanup())
				DebugBreak();
		};
	};
}