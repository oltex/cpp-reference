#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "pair.h"
#include "storage.h"
#include <WinSock2.h>

namespace network {
	class socket final {
	public:
		inline explicit socket(void) noexcept = default;
		inline explicit socket(ADDRESS_FAMILY const af, int const type, int const protocol) noexcept {
			_socket = ::socket(af, type, protocol);
			if (INVALID_SOCKET == _socket)
				DebugBreak();
		}
		inline explicit socket(SOCKET const sock) noexcept
			: _socket(sock) {
		}
		inline explicit socket(socket const& rhs) noexcept = delete;
		inline auto operator=(socket const& rhs) noexcept -> socket & = delete;
		inline explicit socket(socket&& rhs) noexcept
			: _socket(rhs._socket) {
			rhs._socket = INVALID_SOCKET;
		}
		inline auto operator=(socket&& rhs) noexcept -> socket& {
			_socket = rhs._socket;
			rhs._socket = INVALID_SOCKET;
			return *this;
		}
		inline ~socket(void) noexcept {
			closesocket(_socket);
		}
	public:
		inline void bind(storage& stor) const noexcept {
			auto addr = reinterpret_cast<sockaddr*>(&stor.data());
			int len;
			switch (stor.get_family()) {
			case AF_INET:
				len = sizeof(sockaddr_in);
				break;
			}
			if (SOCKET_ERROR == ::bind(_socket, addr, len))
				DebugBreak();
		}
		inline void listen(int backlog) const noexcept {
			if (SOMAXCONN != backlog)
				backlog = SOMAXCONN_HINT(backlog);
			if (SOCKET_ERROR == ::listen(_socket, backlog))
				DebugBreak();
		}
		inline auto accept(void) const noexcept -> pair<socket, storage> {
			sockaddr_storage addr;
			int len = sizeof(sockaddr_in);
			SOCKET sock = ::accept(_socket, reinterpret_cast<sockaddr*>(&addr), &len);
			if (INVALID_SOCKET == sock)
				DebugBreak();
			return pair<socket, storage>(socket(sock), storage(addr));
		}
		inline void connect(storage& stor) const noexcept {
			auto& addr = reinterpret_cast<sockaddr&>(stor.data());
			int len;
			switch (stor.get_family()) {
			case AF_INET:
				len = sizeof(sockaddr_in);
				break;
			}
			if (SOCKET_ERROR == ::connect(_socket, &addr, len))
				DebugBreak();
		}
		inline void shutdown(int const how) const noexcept {
			if (SOCKET_ERROR == ::shutdown(_socket, how))
				DebugBreak();
		}
		inline void close(void) const noexcept {
			closesocket(_socket);
		}
	public:
		inline auto send(char const* const buf, int const len, int const flag) const noexcept -> int {
			return ::send(_socket, buf, len, flag);
		}
		inline auto send_to(char const* const buf, int const len, int const flag, storage& stor) const noexcept -> int {
			auto& addr = reinterpret_cast<sockaddr&>(stor.data());
			return ::sendto(_socket, buf, len, flag, &addr, sizeof(sockaddr_in));
		}
		inline auto receive(char* const buf, int const len, int const flag) const noexcept -> int {
			return ::recv(_socket, buf, len, flag);
		}
		inline auto receive_from(char* const buf, int const len, int const flag) const noexcept -> int {
			//return ::recvfrom(_socket, buf, len, flag);
		}
	public:
		inline void set_tcp_nodelay(int const enable) const noexcept {
			set_option(IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char const*>(&enable), sizeof(enable));
		}
		inline void set_linger(unsigned short const onoff, unsigned short const time) const noexcept {
			LINGER linger{ onoff , time };
			set_option(SOL_SOCKET, SO_LINGER, reinterpret_cast<char const*>(&linger), sizeof(linger));
		}
		inline void set_send_buffer(int const size) const noexcept {
			set_option(SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char const*>(&size), sizeof(size));
		}
		inline void set_receive_buffer(int const size) const noexcept {
			set_option(SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char const*>(&size), sizeof(size));
		}
		inline void set_option(int const level, int const name, char const* value, int const length) const noexcept {
			if (SOCKET_ERROR == setsockopt(_socket, level, name, value, length))
				DebugBreak();
		}
	public:
		inline void set_nonblocking(unsigned long const enable) const noexcept {
			set_io_control(FIONBIO, enable);
		}
		inline void set_io_control(long const cmd, unsigned long arg) const noexcept {
			if (SOCKET_ERROR == ioctlsocket(_socket, cmd, &arg))
				DebugBreak();
		}
	public:
		inline auto data(void) const noexcept -> SOCKET {
			return _socket;
		}
	private:
		SOCKET _socket;
	};
}