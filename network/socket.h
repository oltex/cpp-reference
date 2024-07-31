#pragma once
#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
//#include <WS2tcpip.h>
#include <iostream>



namespace network {
	class socket final {
	public:
		inline explicit socket(address_family const af, type const type, protocol const protocol) noexcept {
			_socket = ::socket(static_cast<int>(af), static_cast<int>(type), static_cast<int>(protocol));
			if (INVALID_SOCKET == _socket)
				error();
		}
		inline ~socket(void) noexcept {
			closesocket(_socket);
		}
	public:
		inline void bind(void) noexcept {
			if (SOCKET_ERROR == ::bind(_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)))
				error();
		}
		inline void listen(void) noexcept {
			if (SOCKET_ERROR == ::listen(_socket, SOMAXCONN_HINT(65535)))
				error();
		}
	private:
		inline void error(void) noexcept {
			std::cout << GetLastError() << std::endl;
			DebugBreak();
		}
	private:
		SOCKET _socket;
	};
}