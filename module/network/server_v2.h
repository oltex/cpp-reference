#pragma once
//#define FD_SETSIZE 7000

#include "session.h"
#include "network.h"
#include "select.h"
#include "file_descriptor.h"
#include "socket.h"
#include <list>

template<typename session_type, typename rpc_type = void>
class server final {
	static_assert(std::is_base_of<session, session_type>::value);
private:
	inline explicit server(void) noexcept
		: _listen(AF_INET, SOCK_STREAM, IPPROTO_TCP) {
		network::storage_ipv4 storage;
		storage.set_address(INADDR_ANY);
		storage.set_port(20000);

		_listen.set_linger(1, 0);
		_listen.set_nonblocking(1);
		//_listen.set_tcp_nodelay(1);

		_listen.bind(reinterpret_cast<network::storage&>(storage));
		_listen.listen(65535);
	}
	inline explicit server(server const&) noexcept = delete;
	inline explicit server(server&&) noexcept = delete;
	inline auto operator=(server const&) noexcept -> server & = delete;
	inline auto operator=(server&&) noexcept -> server & = delete;
	inline ~server(void) noexcept = default;
public:
	inline void execute(void) noexcept {
		timeval time{};
		std::list<session>::iterator first = _session.begin();
		std::list<session>::iterator last = _session.begin();

		_read.zero();
		_write.zero();
		_read.set(_listen);
		while (64 > _read.size() && 64 > _write.size() && last != _session.end()) {
			if (INVALID_SOCKET != last->_socket.data()) {
				_read.set(last->_socket);
				if (0 < last->_send_ring_buffer.size())
					_write.set(last->_socket);
			}
			++last;
		}
		_select.execute(&_read.data(), &_write.data(), nullptr, &time);

		if (_read.is_set(_listen)) {
			while (true) {
				auto [sock, stor] = _listen.accept();
				if (sock.data() == INVALID_SOCKET)
					break;
				auto& iter = _session.emplace_back(std::move(sock), stor);
				if constexpr (!std::is_same_v<session, session_type>)
					iter.accept();
				if constexpr (!std::is_void_v<rpc_type>)
					rpc_type::accept(iter);
			}
		}
		for (;;) {
			while (first != last) {
				//receive
				if (_read.is_set(first->_socket)) {
					int result = first->_socket.receive(
						(char*)(first->_recv_ring_buffer.data() + first->_recv.get_rear())
						, (int)first->_recv.at_once_push(), 0);
					if (SOCKET_ERROR != result && 0 != result) {
						first->_recv_ring_buffer.move_rear(result);
						if constexpr (!std::is_same_v<session, session_type>)
							first->receive();
						if constexpr (!std::is_void_v<rpc_type>)
							rpc_type::receive(*first);
					}
				}
				//send
				if (_write.is_set(first->_socket)) {
					int size = (int)first->_send_ring_buffer.size();
					int deque = (int)first->_send_ring_buffer.at_once_peek();
					size = size < deque ? size : deque;
					int result = first->_socket.send(
						(char*)(first->_send_ring_buffer.data() + first->_send_ring_buffer.get_front()),
						size, 0);
					if (SOCKET_ERROR != result)
						first->_send_ring_buffer.move_front(result);
				}
				//close
				if (INVALID_SOCKET == first->_socket.data()) {
					if constexpr (!std::is_same_v<session, session_type>)
						first->close();
					if constexpr (!std::is_void_v<rpc_type>)
						rpc_type::close(*first);
					first = _session.erase(first);
				}
				else
					++first;
			}
			if (first == _session.end())
				break;
			_read.zero();
			_write.zero();
			while (64 > _read.size() && 64 > _write.size() && last != _session.end()) {
				if (INVALID_SOCKET != last->_socket.data()) {
					_read.set(last->_socket);
					if (0 < last->_send_ring_buffer.size())
						_write.set(last->_socket);
				}
				++last;
			}
			if (0 != _read.size() || 0 != _write.size())
				_select.execute(&_read.data(), &_write.data(), nullptr, &time);
		}
	}
private:
	network::network _network;
	network::select _select;
	network::file_descriptor _read;
	network::file_descriptor _write;

	network::socket _listen;
	std::list<session> _session;
};