#pragma once
#include "library/system/socket.h"
#include "library/container/vector.h"
#include "library/container/array.h"

namespace framework {
	struct network;
	struct accept {
		library::socket _socket;
		library::overlap _overlap;
		library::array<char, 64> _buffer;

		inline explicit accept(void) noexcept = default;
		inline explicit accept(accept const&) noexcept = delete;
		inline explicit accept(accept&&) noexcept = delete;
		inline auto operator=(accept const&) noexcept -> accept & = delete;
		inline auto operator=(accept&&) noexcept -> accept & = delete;
		inline ~accept(void) noexcept = default;

		inline void create(void) noexcept {
			_socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
		}
		inline void close(void) noexcept {
			_socket.close();
		}
		inline auto inherit(framework::network& network) noexcept;
		inline auto address(void) noexcept {
			return library::socket::get_accept_ex_socket_address(_buffer.data())._second;
		};
		inline static auto recover(OVERLAPPED* overlapped) noexcept -> accept& {
			return *reinterpret_cast<accept*>(reinterpret_cast<unsigned char*>(library::overlap::recover(overlapped)) - offsetof(accept, _overlap));
		}
	};
	struct connect {
		library::socket _socket;
		library::overlap _overlap;

		inline explicit connect(void) noexcept = default;
		inline explicit connect(connect const&) noexcept = delete;
		inline explicit connect(connect&&) noexcept = delete;
		inline auto operator=(connect const&) noexcept -> connect & = delete;
		inline auto operator=(connect&&) noexcept -> connect & = delete;
		inline ~connect(void) noexcept = default;

		inline static auto recover(OVERLAPPED* overlapped) noexcept -> accept& {
			return *reinterpret_cast<accept*>(reinterpret_cast<unsigned char*>(library::overlap::recover(overlapped)) - offsetof(connect, _overlap));
		}
	};

	struct network final {
		using size_type = unsigned int;
		library::socket _listen;
		library::vector<accept> _accept;
		library::vector<connect> _connect;

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
				auto& accept = _accept.emplace_back();
				accept.create();
				_listen.accept(accept._socket, accept._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, accept._overlap);
			}
		}
		inline void accept(framework::accept& accept) noexcept {
			accept._socket.close();
			accept._socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
			_listen.accept(accept._socket, accept._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, accept._overlap);
		}
		inline void connect(size_type capacity) noexcept {
			_connect.reserve(capacity);
			for (size_type index = 0; index < capacity; ++index) {
				auto& connect = _connect.emplace_back();
				//accept.create();
				//_listen.accept(accept._socket, accept._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, accept._overlap);
			}
		}

		//inline void finalize(void) noexcept {
		//	_socket.close();
		//	//_vector.clear();
		//}


	
	};
	inline auto accept::inherit(framework::network& network) noexcept {
		_socket.set_option_update_accept_context(network._listen);
	}

}