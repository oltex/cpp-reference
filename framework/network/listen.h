#pragma once
#include "library/socket.h"
#include "library/vector.h"
#include "library/array.h"

namespace framework {
	struct listen;
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

		inline auto initialize(framework::listen& listen) noexcept;
		inline static auto recover(OVERLAPPED* overlapped) noexcept -> accept& {
			return *reinterpret_cast<accept*>(reinterpret_cast<unsigned char*>(overlapped) - offsetof(library::overlap, _overlapped) - offsetof(accept, _overlap));
		}
	};

	struct listen final {
		library::socket _socket;
		using size_type = unsigned int;
		using iterator = library::template vector<accept>::iterator;
		library::vector<accept> _vector;

		inline explicit listen(void) noexcept = default;
		inline explicit listen(listen const&) noexcept = delete;
		inline explicit listen(listen&&) noexcept = delete;
		inline auto operator=(listen const&) noexcept -> listen & = delete;
		inline auto operator=(listen&&) noexcept -> listen & = delete;
		inline ~listen(void) noexcept = default;

		inline void initialize(char const* const ip, unsigned short port, int backlog) noexcept {
			_socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
			_socket.set_option_linger(1, 0);
			_socket.set_option_send_buffer(0);
			library::socket_address_ipv4 sockaddr;
			sockaddr.set_address(ip);
			sockaddr.set_port(port);
			_socket.bind(sockaddr);
			_socket.listen(backlog);
		}
		inline void finalize(void) noexcept {
			_socket.close();
			//_vector.clear();
		}

		inline void accept(size_type capacity) noexcept {
			_vector.reserve(capacity);
			for (size_type index = 0; index < capacity; ++index) {
				auto& accept_ = _vector.emplace_back();
				accept_._socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
				_socket.accept(accept_._socket, accept_._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, accept_._overlap);
			}
		}
		inline void accept(framework::accept& accept) noexcept {
			accept._socket.close();
			accept._socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
			_socket.accept(accept._socket, accept._buffer.data(), sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, accept._overlap);
		}
	};

	inline auto accept::initialize(framework::listen& listen) noexcept {
		_socket.set_option_update_accept_context(listen._socket);
		return library::socket::get_accept_ex_socket_address(_buffer.data())._second;

	}
}