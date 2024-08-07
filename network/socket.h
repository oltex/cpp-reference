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
			auto& addr = reinterpret_cast<sockaddr&>(stor.get_storage());
			if (SOCKET_ERROR == ::connect(_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)))
				DebugBreak();
		}
		inline void close(void) const noexcept {
			closesocket(_socket);
		}
	public:
		inline void send(char const* const buf, int const len, int const flag) const noexcept {
			int ret = ::send(_socket, buf, len, flag);
			if (SOCKET_ERROR == ret)
				DebugBreak();
		}
		inline void sendto(char const* const buf, int const len, int const flag) const noexcept {
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
	public:
		inline void set_tcp_nodelay(int const optval) const noexcept {
			set_option(IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char const*>(&optval), sizeof(optval));
		}
		inline void set_linger(LINGER linger) const noexcept {
			set_option(SOL_SOCKET, SO_LINGER, reinterpret_cast<char const*>(&linger), sizeof(linger));
		}
		inline void set_option(int const level, int const name, char const* value, int const length) const noexcept {
			if (SOCKET_ERROR == setsockopt(_socket, level, name, value, length))
				DebugBreak();
		}
	private:
		SOCKET _socket;
	};
}