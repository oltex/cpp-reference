#pragma once
#include "../network.h"
#include "../../input_output/completion.h"
#include "../../multi/thread.h"

#include "../../../data-structure/vector/vector.h"

class server final {
private:
	class session_array final {
		inline auto begin(void) noexcept -> node* {
			return _vector.begin();
		}
		inline auto end(void) noexcept -> node* {
			return _vector.end();
		}
		inline auto data(void) -> node* {
			return _vector.data();
		}
	private:
		node* _head = nullptr;
		data_structure::vector<node> _vector;
	};
public:
	inline explicit server(void) noexcept = default;
	inline explicit server(server const& rhs) noexcept = delete;
	inline explicit server(server&& rhs) noexcept = delete;
	inline auto operator=(server const& rhs) noexcept -> server & = delete;
	inline auto operator=(server&& rhs) noexcept -> server & = delete;
	inline ~server(void) noexcept = default;
public:
	inline void accept(void) noexcept {
		for (;;) {
			auto [socket, storage] = _listen.accept();
			if (INVALID_SOCKET == socket.data())
				return;
				if (false == on_accept(storage))
					continue;

				session& session_ = _session.get();
				session_.initialize(std::move(socket), storage);

			//	session& session_ = _session.get();
			//	session_.initialize(std::move(socket), storage);

			//	_completion.connect_port(session_._socket, reinterpret_cast<ULONG_PTR>(&session_));

			//	on_create(reinterpret_cast<unsigned long long&>(session_._key));
			//	if (true == receive(session_))
			//		continue;
			//	if (0 == InterlockedDecrement(&session_._io_count)) {
			//		session_.finalize();
			//		_session.release(session_);
			//	}
		}
	}
public:
	inline virtual bool on_accept(network::storage& storage) noexcept {
		return true;
	}
private:
	network::network _network;
	input_output::completion _completion;
	data_structure::vector<multi::thread> _thread;

	network::socket _listen;
	//session_array _session;
	//unsigned long long _accept_count = 0;
};