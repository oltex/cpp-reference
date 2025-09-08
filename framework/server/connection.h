#pragma once
#include "library/system/socket.h"
#include "library/container/vector.h"
#include "library/container/array.h"
#include "library/container/lockfree/free_list.h"

namespace framework {
	class connection {
	public:
		library::socket _socket;
		library::overlap _overlap;
		library::socket_address_ipv4 _address;
		library::array<char, 64> _buffer;

		inline explicit connection(void) noexcept = default;
		inline explicit connection(connection const&) noexcept = delete;
		inline explicit connection(connection&&) noexcept = delete;
		inline auto operator=(connection const&) noexcept -> connection & = delete;
		inline auto operator=(connection&&) noexcept -> connection & = delete;
		inline ~connection(void) noexcept = default;

		inline void create_socket(void) noexcept {
			_socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
		}
		inline void close_socket(void) noexcept {
			_socket.close();
		}
		inline void listen_socket(char const* const ip, unsigned short port, int backlog) noexcept {
			_socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
			_socket.set_option_linger(1, 0);
			_socket.set_option_send_buffer(0);
			library::socket_address_ipv4 sockaddr;
			sockaddr.ip(ip);
			sockaddr.port(port);
			_socket.bind(sockaddr);
			_socket.listen(backlog);
		}
		inline bool accept_post(connection& connection) noexcept {
			switch (_socket.accept(connection._socket, connection._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, connection._overlap)) {
				using enum library::socket::result;
			case pending:
			case complet:
				return true;
			case close:
				return false;
			}
		}
		inline void accept_inherit(connection& connection) const noexcept {
			_socket.set_option_update_accept_context(connection._socket);
		}
		inline auto accept_address(void) noexcept -> library::socket_address_ipv4 {
			return library::socket::get_accept_ex_socket_address(_buffer.data())._second;
		};
		inline void connect_inherit(connection& connection) const noexcept {
			_socket.set_option_update_connect_context(connection._socket);
		}
		inline bool connect_post(char const* const ip, unsigned short port) noexcept {
			library::socket_address_ipv4 address;
			address.ip(ip);
			address.port(port);
			switch (_socket.connect(address, _overlap)) {
				using enum library::socket::result;
			case pending:
			case complet:
				return true;
			case close:
				return false;
			}
		}
		inline auto connect_address(void) noexcept -> library::socket_address_ipv4& {
			return _address;
		}

		inline static auto recover(OVERLAPPED* overlapped) noexcept -> connection& {
			return *reinterpret_cast<connection*>(reinterpret_cast<unsigned char*>(library::overlap::recover(overlapped)) - offsetof(connection, _overlap));
		}
	};
	class connection_array final : public library::lockfree::free_list<connection, true, true> {
		using size_type = unsigned int;
		using base = library::lockfree::free_list<connection, true, true>;
		size_type _size;
	public:
		inline explicit connection_array(size_type const capacity) noexcept
			: base(capacity), _size(0) {
		};
		inline explicit connection_array(connection_array const&) noexcept = delete;
		inline explicit connection_array(connection_array&&) noexcept = delete;
		inline auto operator=(connection_array const&) noexcept -> connection_array & = delete;
		inline auto operator=(connection_array&&) noexcept -> connection_array & = delete;
		inline ~connection_array(void) noexcept = default;

		inline auto allocate(void) noexcept -> connection* {
			auto result = base::allocate();
			if (nullptr != result)
				library::interlock_increment(_size);
			return result;
		}
		inline void deallocate(connection* value) noexcept {
			base::deallocate(value);
			library::interlock_decrement(_size);
		}
	};
}


/*struct network final {
	using size_type = unsigned int;
	library::socket _listen_socket;
	library::vector<connection> _accept_array;
	size_type _accept_size;
	library::lockfree::free_list<connection> _connect_array;
	size_type _connect_size;

	inline explicit network(size_type connection) noexcept
		: _accept_size(0), _connect_connection(connection) {
		_accept_connection.reserve(connection);
	};
	inline explicit network(network const&) noexcept = delete;
	inline explicit network(network&&) noexcept = delete;
	inline auto operator=(network const&) noexcept -> network & = delete;
	inline auto operator=(network&&) noexcept -> network & = delete;
	inline ~network(void) noexcept = default;

	inline void listen_start(void) noexcept {
		_accept_size = _accept_array.capacity();
		for (size_type index = 0; index < _accept_array.capacity(); ++index) {
			auto& connection = _accept_array.emplace_back();
			_listen_socket.accept(connection._socket, connection._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, connection._overlap);
		}
	}
	inline void listen_stop(void) noexcept {
		_listen.close();
		library::sleep(1000);
		_accept.clear();
	}
	inline auto ready_connect(void) noexcept -> framework::connection& {
		return *_connect.allocate();
	}
	inline void release_connection(framework::connection& connection) noexcept {
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
};*/