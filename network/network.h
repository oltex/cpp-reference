#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <intrin.h>

namespace network {
	class network {
	public:
		inline explicit network(void) noexcept {
			WSAData wsadata;
			if (0 != WSAStartup(0x0202, &wsadata))
				__debugbreak();
		};
		inline explicit network(network const& rhs) noexcept = delete;
		inline explicit network(network&& rhs) noexcept = delete;
		inline auto operator=(network const& rhs) noexcept -> network & = delete;
		inline auto operator=(network&& rhs) noexcept -> network & = delete;
		inline ~network(void) noexcept {
			if (SOCKET_ERROR == WSACleanup())
				__debugbreak();
		};
	};
}