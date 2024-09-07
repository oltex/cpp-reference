#pragma once
#include "socket.h"
#include "storage.h"
#include "ring_buffer.h"
#include "serialize_buffer.h"
#include <optional>

class session {
	friend class server;
	friend class remote_procedure_call;
	struct header {
		unsigned short _size;
	};
public:
	inline explicit session(network::socket&& socket, network::storage& storage)
		: _socket(std::move(socket)), _storage(storage) {
	}
	inline ~session(void) noexcept = default;
public:
	inline void accept(void) noexcept {
	};
	inline void receive(void) noexcept {
		while (true) {
			if (!_header) {
				if (sizeof(header) > _recv_ring_buffer.size())
					break;
				header header_;
				_recv_ring_buffer.peek((unsigned char*)&header_, sizeof(header));
				_recv_ring_buffer.pop(sizeof(header));
				_header = header_;
			}
			else {
				if (_header->_size > _recv_ring_buffer.size())
					break;
				data_structure::serialize_buffer serialize_buffer;
				_recv_ring_buffer.peek((unsigned char*)serialize_buffer.data(), _header->_size);
				_recv_ring_buffer.pop(_header->_size);
				serialize_buffer.move_rear(_header->_size);
				_header = std::nullopt;


			}
		}
	};
	inline void send(data_structure::serialize_buffer& serialize_buffer) noexcept {
		_send_ring_buffer.push(serialize_buffer.data(), serialize_buffer.rear());
	};
	inline void close(void) noexcept {
	};
private:
	network::socket _socket;
	network::storage _storage;
	data_structure::ring_buffer _recv_ring_buffer;
	data_structure::ring_buffer _send_ring_buffer;
	std::optional<header> _header;
};