#pragma once
#pragma comment(lib,"ws2_32.lib")

#include "storage.h"

#include <WinSock2.h>

namespace network {
	class socket final {
	public:
		inline explicit socket(ADDRESS_FAMILY const af, int const type, int const protocol) noexcept {
			_socket = ::socket(af, type, protocol);
			if (INVALID_SOCKET == _socket)
				DebugBreak();
		}
		inline explicit socket(SOCKET const sock) noexcept
			: _socket(sock) {
		}
		inline ~socket(void) noexcept {
			closesocket(_socket);
		}
	public:
		inline void bind(storage& stor) const noexcept {
			auto addr = reinterpret_cast<sockaddr*>(&stor.get_storage());
			if (SOCKET_ERROR == ::bind(_socket, addr, sizeof(sockaddr_in)))
				DebugBreak();
		}
		inline void listen(int backlog) const noexcept {
			if (SOMAXCONN != backlog)
				backlog = SOMAXCONN_HINT(backlog);
			if (SOCKET_ERROR == ::listen(_socket, backlog))
				DebugBreak();
		}
		inline auto accept(void) const noexcept -> socket {
			sockaddr addr;
			int len = sizeof(sockaddr_in);
			SOCKET sock = ::accept(_socket, reinterpret_cast<sockaddr*>(&addr), &len);
			if (INVALID_SOCKET == sock)
				DebugBreak();
			return socket(sock);
		}
		inline void connect(storage& stor) const noexcept {
			auto addr = reinterpret_cast<sockaddr*>(&stor.get_storage());
			if (SOCKET_ERROR == ::connect(_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)))
				DebugBreak();
		}
		inline void close(void) const noexcept {
			closesocket(_socket);
		}
		//inline void set_option(void) const noexcept {
		//	int opt_val = TRUE;
		//	setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt_val, sizeof(opt_val));
		//}
	public:
		inline void send(char const* const buf, int const len, int const flag) const noexcept {
			int ret = ::send(_socket, buf, len, flag);
			if (SOCKET_ERROR == ret)
				DebugBreak();
		}
		inline auto recv(char* const buf, int const len, int const flag) const noexcept -> int {
			int ret = ::recv(_socket, buf, len, flag);
			if (SOCKET_ERROR == ret)
				DebugBreak();
			return ret;
		}
	private:
		SOCKET _socket;
	};
}