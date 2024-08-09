#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>

namespace network {
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