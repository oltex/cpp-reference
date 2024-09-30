#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "storage.h"
#include "pair.h"
#include <WinSock2.h>

namespace network {
	class socket final {
	public:
		inline explicit socket(ADDRESS_FAMILY const af, int const type, int const protocol) noexcept {
			_socket = ::socket(af, type, protocol);
			if (INVALID_SOCKET == _socket) {
				switch (GetLastError()) {
				default:
					DebugBreak();
				}
			}
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
		inline auto bind(storage& stor) const noexcept -> int {
			auto addr = reinterpret_cast<sockaddr*>(&stor.data());
			int len;
			switch (stor.get_family()) {
			case AF_INET:
				len = sizeof(sockaddr_in);
				break;
			}
			int result = ::bind(_socket, addr, len);
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
					//WSAEADDRINUSE
				default:
					DebugBreak();
				}
			}
			return result;
		}
		inline auto listen(int backlog) const noexcept -> int {
			if (SOMAXCONN != backlog)
				backlog = SOMAXCONN_HINT(backlog);
			int result = ::listen(_socket, backlog);
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				default:
					DebugBreak();
				}
			}
			return result;
		}
		inline auto accept(void) const noexcept -> data_structure::pair<socket, storage> {
			sockaddr_storage addr;
			int len = sizeof(sockaddr_in);
			SOCKET sock = ::accept(_socket, reinterpret_cast<sockaddr*>(&addr), &len);
			if (INVALID_SOCKET == sock) {
				switch (GetLastError()) {
				case WSAEWOULDBLOCK:
					break;
				default:
					DebugBreak();
				}
			}
			return data_structure::pair<socket, storage>(socket(sock), storage(addr));
		}
		inline auto connect(storage& stor) noexcept -> int {
			auto& addr = reinterpret_cast<sockaddr&>(stor.data());
			int length;
			switch (stor.get_family()) {
			case AF_INET:
				length = sizeof(sockaddr_in);
				break;
			}
			int result = ::connect(_socket, &addr, length);
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				case WSAEWOULDBLOCK:
					break;
				case WSAETIMEDOUT:
				case WSAECONNREFUSED:
					close();
					break;
				default:
					DebugBreak();
				}
			}
			return result;
		}
		inline void shutdown(int const how) const noexcept {
			if (SOCKET_ERROR == ::shutdown(_socket, how))
				DebugBreak();
		}
		inline void close(void) noexcept {
			closesocket(_socket);
			_socket = INVALID_SOCKET;
		}
	public:
		inline auto send(char const* const buf, int const len, int const flag) noexcept -> int {
			int result = ::send(_socket, buf, len, flag);
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				case WSAEWOULDBLOCK:
					break;
				case WSAECONNRESET:
				case WSAECONNABORTED:
					close();
					break;
				case WSAENOTCONN:
				default:
					DebugBreak();
				}
			}
			return result;
		}
		inline auto send_to(char const* const buf, int const len, int const flag, storage& stor) const noexcept -> int {
			auto& addr = reinterpret_cast<sockaddr&>(stor.data());
			return ::sendto(_socket, buf, len, flag, &addr, sizeof(sockaddr_in));
		}
		inline auto receive(char* const buf, int const len, int const flag) noexcept -> int {
			int result = ::recv(_socket, buf, len, flag);
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				case WSAEWOULDBLOCK:
					break;
				case WSAECONNRESET:
				case WSAECONNABORTED:
					close();
					break;
				case WSAENOTCONN:
				default:
					DebugBreak();
				}
			}
			else if (0 == result)
				close();
			return result;
		}
		inline auto receive_from(char* const buf, int const len, int const flag, storage& stor, int& length) noexcept -> int {
			auto& addr = reinterpret_cast<sockaddr&>(stor.data());
			return ::recvfrom(_socket, buf, len, flag, &addr, &length);
		}
	public:
		inline void set_tcp_nodelay(int const enable) const noexcept {
			set_option(IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char const*>(&enable), sizeof(int));
		}
		inline void set_linger(unsigned short const onoff, unsigned short const time) const noexcept {
			LINGER linger{ onoff , time };
			set_option(SOL_SOCKET, SO_LINGER, reinterpret_cast<char const*>(&linger), sizeof(LINGER));
		}
		inline void set_broadcast(unsigned long const enable) const noexcept {
			set_option(SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char const*>(&enable), sizeof(unsigned long));
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