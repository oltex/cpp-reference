#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "storage.h"
#include "../../data-structure/pair/pair.h"
#include <WinSock2.h>
#include <intrin.h>

namespace network {
	class socket final {
	public:
		inline explicit socket(void) noexcept
			: _socket(INVALID_SOCKET) {
		}
		inline explicit socket(ADDRESS_FAMILY const af, int const type, int const protocol) noexcept {
			_socket = ::socket(af, type, protocol);
			if (INVALID_SOCKET == _socket) {
				//switch (GetLastError()) {
				//default:
				//	__debugbreak();
				//}
			}
		}
		inline explicit socket(SOCKET const sock) noexcept
			: _socket(sock) {
		}
		inline explicit socket(socket const& rhs) noexcept = delete;
		inline explicit socket(socket&& rhs) noexcept
			: _socket(rhs._socket) {
			rhs._socket = INVALID_SOCKET;
		}
		inline auto operator=(socket const& rhs) noexcept -> socket & = delete;
		inline auto operator=(socket&& rhs) noexcept -> socket& {
			closesocket(_socket);
			_socket = rhs._socket;
			rhs._socket = INVALID_SOCKET;
			return *this;
		};
		inline ~socket(void) noexcept {
			closesocket(_socket);
		}
	public:
		inline auto bind(storage& stor) const noexcept -> int {
			int result = ::bind(_socket, reinterpret_cast<sockaddr*>(&stor.data()), stor.get_length());
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				case WSAEADDRINUSE:
				default:
					__debugbreak();
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
				case WSAEINVAL:
				default:
					__debugbreak();
				}
			}
			return result;
		}
		inline auto accept(void) const noexcept -> data_structure::pair<socket, storage> {
			sockaddr_storage addr;
			int length = sizeof(sockaddr_in);
			SOCKET sock = ::accept(_socket, reinterpret_cast<sockaddr*>(&addr), &length);
			if (INVALID_SOCKET == sock) {
				switch (GetLastError()) {
				case WSAEWOULDBLOCK:
					break;
				case WSAEINVAL:
				default:
					__debugbreak();
				}
			}
			return data_structure::pair<socket, storage>(socket(sock), storage(addr));
		}
		inline auto connect(storage& stor) noexcept -> int {
			int result = ::connect(_socket, reinterpret_cast<sockaddr*>(&stor.data()), stor.get_length());
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				case WSAEWOULDBLOCK:
					break;
				case WSAETIMEDOUT:
				case WSAECONNREFUSED:
					close();
					break;
				default:
					__debugbreak();
				}
			}
			return result;
		}
		inline void shutdown(int const how) const noexcept {
			if (SOCKET_ERROR == ::shutdown(_socket, how))
				__debugbreak();
		}
		inline void close(void) noexcept {
			closesocket(_socket);
			_socket = INVALID_SOCKET;
		}
	public:
		inline auto send(char const* const buffer, int const length, int const flag) noexcept -> int {
			int result = ::send(_socket, buffer, length, flag);
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
					__debugbreak();
				}
			}
			return result;
		}
		inline auto send_to(char const* const buffer, int const length, int const flag, storage& stor) const noexcept -> int {
			auto& addr = reinterpret_cast<sockaddr&>(stor.data());
			return ::sendto(_socket, buffer, length, flag, &addr, sizeof(sockaddr_in));
		}
		inline auto wsa_send(WSABUF* buffer, unsigned long count, unsigned long* byte, unsigned long flag, _OVERLAPPED* overlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE completion_roution) noexcept -> int {
			int result = WSASend(_socket, buffer, count, byte, flag, overlapped, completion_roution);
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				case WSA_IO_PENDING:
					break;
				case WSAECONNRESET:
				case WSAECONNABORTED:
					close();
					break;
				default:
					__debugbreak();
				}
			}
			return result;
		}
		inline auto receive(char* const buffer, int const length, int const flag) noexcept -> int {
			int result = ::recv(_socket, buffer, length, flag);
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
					__debugbreak();
				}
			}
			else if (0 == result)
				close();
			return result;
		}
		inline auto receive_from(char* const buffer, int const length, int const flag, storage& stor, int& from_length) noexcept -> int {
			auto& addr = reinterpret_cast<sockaddr&>(stor.data());
			return ::recvfrom(_socket, buffer, length, flag, &addr, &from_length);
		}
		inline auto wsa_receive(WSABUF* buffer, unsigned long count, unsigned long* byte, unsigned long* flag, _OVERLAPPED* overlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE completion_roution) noexcept -> int {
			int result = WSARecv(_socket, buffer, count, byte, flag, overlapped, completion_roution);
			if (SOCKET_ERROR == result) {
				switch (GetLastError()) {
				case WSA_IO_PENDING:
					break;
				case WSAECONNRESET:
				case WSAECONNABORTED:
					close();
					break;
				case WSAENOTSOCK:
				default:
					__debugbreak();
				}
			}
			return result;
		}
		inline bool wsa_get_overlapped_result(_OVERLAPPED* overlapped, unsigned long* transfer, bool const wait, unsigned long* flag) noexcept {
			return WSAGetOverlappedResult(_socket, overlapped, transfer, wait, flag);
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
				__debugbreak();
		}
	public:
		inline void set_nonblocking(unsigned long const enable) const noexcept {
			set_io_control(FIONBIO, enable);
		}
		inline void set_io_control(long const cmd, unsigned long arg) const noexcept {
			if (SOCKET_ERROR == ioctlsocket(_socket, cmd, &arg))
				__debugbreak();
		}
	public:
		inline auto get_local_storage(void) noexcept -> storage {
			storage stor;
			int length = sizeof(sockaddr_in);
			getsockname(_socket, reinterpret_cast<sockaddr*>(&stor.data()), &length);
			return stor;
		}
		inline auto get_remote_storage(void) noexcept -> storage {
			storage stor;
			int length = sizeof(sockaddr_in);
			getpeername(_socket, reinterpret_cast<sockaddr*>(&stor.data()), &length);
			return stor;
		}
	public:
		inline auto data(void) noexcept -> SOCKET& {
			return _socket;
		}
	private:
		SOCKET _socket;
	};
}