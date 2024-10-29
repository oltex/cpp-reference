#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <intrin.h>

namespace network {
	inline static void start_up(void) noexcept {
		WSAData wsadata;
		if (0 != WSAStartup(0x0202, &wsadata))
			__debugbreak();
	};
	inline static void clean_up(void) noexcept {
		if (SOCKET_ERROR == WSACleanup())
			__debugbreak();
	};
}