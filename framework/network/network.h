#pragma once
#include "library/system/socket.h"
#include "library/container/vector.h"
#include "library/container/array.h"

namespace framework {
	struct network;
	struct connection {
		library::socket _socket;
		library::overlap _overlap;
		library::array<char, 64> _buffer;

		inline explicit connection(void) noexcept {
			_socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
		};
		inline explicit connection(connection const&) noexcept = delete;
		inline explicit connection(connection&&) noexcept = delete;
		inline auto operator=(connection const&) noexcept -> connection & = delete;
		inline auto operator=(connection&&) noexcept -> connection & = delete;
		inline ~connection(void) noexcept = default;

		inline void create(void) noexcept {
			_socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
		}
		inline void close(void) noexcept {
			_socket.close();
		}
		inline void connect(void) noexcept {
			//_socket.connect()
		}
		inline auto inherit(framework::network& network) noexcept;
		inline auto address(void) noexcept -> library::socket_address_ipv4 {
			return library::socket::get_accept_ex_socket_address(_buffer.data())._second;
		};
		inline static auto recover(OVERLAPPED* overlapped) noexcept -> connection& {
			return *reinterpret_cast<connection*>(reinterpret_cast<unsigned char*>(library::overlap::recover(overlapped)) - offsetof(connection, _overlap));
		}
	};

	struct network final {
		using size_type = unsigned int;
		library::socket _listen;
		library::vector<connection> _accept;
		library::vector<connection> _connect;

		inline explicit network(void) noexcept = default;
		inline explicit network(network const&) noexcept = delete;
		inline explicit network(network&&) noexcept = delete;
		inline auto operator=(network const&) noexcept -> network & = delete;
		inline auto operator=(network&&) noexcept -> network & = delete;
		inline ~network(void) noexcept = default;

		inline void listen(char const* const ip, unsigned short port, int backlog) noexcept {
			_listen.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
			_listen.set_option_linger(1, 0);
			_listen.set_option_send_buffer(0);
			library::socket_address_ipv4 sockaddr;
			sockaddr.set_address(ip);
			sockaddr.set_port(port);
			_listen.bind(sockaddr);
			_listen.listen(backlog);
		}
		inline void accept(size_type capacity) noexcept {
			_accept.reserve(capacity);
			for (size_type index = 0; index < capacity; ++index) {
				auto& connection = _accept.emplace_back();
				_listen.accept(connection._socket, connection._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, connection._overlap);
			}
		}
		inline void accept(framework::connection& connection) noexcept {
			connection.close();
			connection.create();
			switch (_listen.accept(connection._socket, connection._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, connection._overlap)) {
				using enum library::socket::result;
			case complet:
			case pending:
				break;
			case close:
				connection._socket.close();
				break;
			}
		}
		inline void close(void) noexcept {
			_listen.close();
			//while (0 != _accept_count) {
			//}
			//_accept.clear();
		}

		inline void connect(/*size_type capacity*/) noexcept {
			_connect.reserve(1);
			for (size_type index = 0; index < 1; ++index) {
				auto& connect = _connect.emplace_back();
				connect.create();
				connect.connect()
				//_listen.accept(accept._socket, accept._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, accept._overlap);
			}
		}
	};
	inline auto connection::inherit(framework::network& network) noexcept {
		_socket.set_option_update_accept_context(network._listen);
	}

}