#pragma once
#include "library/system/socket.h"
#include "library/container/vector.h"
#include "library/container/array.h"
#include "library/container/lockfree/free_list.h"

namespace framework {
	struct network;
	struct connection {
		library::socket _socket;
		library::overlap _overlap;
		union {
			library::socket_address_ipv4 _address;
			library::array<char, 64> _buffer;
		};
		inline explicit connection(void) noexcept
			: _socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED){
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
		inline auto address(bool accept) noexcept -> library::socket_address_ipv4 {
			if (accept)
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
		size_type _accept_size;
		library::lockfree::free_list<connection> _connect;

		inline explicit network(size_type connection) noexcept 
			: _accept_size(0), _connect(connection){
			_accept.reserve(connection);
		};
		inline explicit network(network const&) noexcept = delete;
		inline explicit network(network&&) noexcept = delete;
		inline auto operator=(network const&) noexcept -> network & = delete;
		inline auto operator=(network&&) noexcept -> network & = delete;
		inline ~network(void) noexcept = default;

		inline void ready_listen(char const* const ip, unsigned short port, int backlog) noexcept {
			_listen.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
			_listen.set_option_linger(1, 0);
			_listen.set_option_send_buffer(0);
			library::socket_address_ipv4 sockaddr;
			sockaddr.ip(ip);
			sockaddr.port(port);
			_listen.bind(sockaddr);
			_listen.listen(backlog);
		}
		inline void start_listen(void) noexcept {
			_accept_size = _accept.capacity();
			for (size_type index = 0; index < _accept.capacity(); ++index) {
				auto& connection = _accept.emplace_back();
				_listen.accept(connection._socket, connection._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, connection._overlap);
			}
		}
		inline void stop_listen(void) noexcept {
			_listen.close();
			library::sleep(1000);
			_accept.clear();
		}
		inline void connect_socket(void) noexcept {
			auto connection = _connect.allocate();
		}
		inline void release_connection(bool accept, framework::connection& connection) noexcept {
			if (accept) {
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
		}
	};
	inline auto connection::inherit(framework::network& network) noexcept {
		_socket.set_option_update_accept_context(network._listen);
	}
}