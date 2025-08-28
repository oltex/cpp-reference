#pragma once
#include "iocp.h"
#include "network.h"
#include "session.h"

namespace framework {
	class server final : iocp::object {
		using size_type = unsigned int;
		enum class task : unsigned char {
			accept = 0, connect, session, destory, function
		};
		iocp& _iocp;
		network _network;
		session_array _session_array;
	public:
		inline explicit server(void) noexcept
			: _iocp(framework::iocp::instance()) {
		}
		inline explicit server(server const&) noexcept = delete;
		inline explicit server(server&&) noexcept = delete;
		inline auto operator=(server const&) noexcept -> server & = delete;
		inline auto operator=(server&&) noexcept -> server & = delete;
		inline ~server(void) noexcept = default;

		inline void start(void) noexcept {
			_session_array.initialize(100);
		}
		inline void stop(void) noexcept {
			_session_array.finalize();
		}

		inline void accept(char const* const ip, unsigned short port, int backlog) noexcept {
			_network.listen(ip, port, backlog);
			_iocp.connect(*this, _network._listen, static_cast<uintptr_t>(task::accept));
			_network.accept(16);
		}
		//inline void reject(void) {
			//_listen.finalize();
		//}
		inline void connect(char const* const ip, unsigned short port) noexcept {
			//_socket.create(AF_INET, SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED);
			//_socket.set_option_linger(1, 0);
			//_socket.set_option_send_buffer(0);
			//library::socket_address_ipv4 sockaddr;
			//sockaddr.set_address(ip);
			//sockaddr.set_port(port);
			//_socket.bind(sockaddr);

			//socket.connect()
			//system_component::socket_address_ipv4 socket_address;
			//socket_address.set_address(address);
			//socket_address.set_port(port);
			//system_component::socket socket(AF_INET, SOCK_STREAM, 0);
			//socket.set_linger(1, 0);
			//socket.connect(socket_address);

			//session& session_ = *_session_array.acquire();
			//session_.initialize(std::move(socket), _timeout_duration);
			//_complation_port.connect(session_._socket, reinterpret_cast<ULONG_PTR>(&session_));

			//if (!session_.receive() && session_.release()) {
			//	on_destroy_session(session_._key);
			//	_session_array.release(session_);
			//	return 0;
			//}
			//return session_._key;
		}
	private:
		inline void worker(bool result, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept override {
			switch (static_cast<task>(key)) {
			case task::accept: {
				auto& accept = accept::recover(overlapped);
				if (false == result)
					accept.close();
				else {
					accept.inherit(_network);
					if (auto address = accept.address(); true == on_accept_socket(address)) {
						auto& session = *_session_array.allocate();
						session.initialize(accept, 400000);
						_iocp.connect(*this, session._socket, static_cast<uintptr_t>(task::session));

						on_create_session(session._key);
						if (!session.receive() && session.release<false>()) {
							on_destroy_session(session._key);
							_session_array.deallocate(&session);
						}
					}
					_network.accept(accept);
				}
			} break;
			case task::connect: {
				auto& connect = connect::recover(overlapped);
				if (false == result)
					connect._socket.close();
				else {

				}

			} break;
			case task::session: {
				auto [session, task] = session::recover(overlapped);
				if (session::task::recv == task) {
					if (0 != transferred) {
						session._receive_message.move_rear(transferred);
						for (;;) {
							if (auto message = session.message(); !message)
								break;
							else if (false == on_receive_session(session._key, *message)) {
								session.cancel();
								break;
							}
						}
						if (session.receive())
							break;
					}
					if (session.release<false>()) {
						on_destroy_session(session._key);
						_session_array.deallocate(&session);
					}
				}
				else {
					if (0 != transferred) {
						session.flush();
						if (session.send())
							break;
					}
					if (session.release<true>()) {
						on_destroy_session(session._key);
						_session_array.deallocate(&session);
					}
				}
			} break;
			case task::destory: {
			} break;
			case task::function: {
			} break;
			}
		};

		inline virtual bool on_accept_socket(library::socket_address_ipv4& socket_address) noexcept {
			return true;
		}
		inline virtual void on_create_session(unsigned long long key) noexcept {
			//message_pointer message_ = server::create_message();
			//*message_ << 0x7fffffffffffffff;
			//do_send_session(key, message_);
		}
		inline virtual bool on_receive_session(unsigned long long key, message& message) noexcept {
			unsigned long long value;
			message >> value;
			auto message_ = create_message(10);
			message_ << value;

			do_send_session(key, message_);
			return true;
		};
		inline virtual void on_destroy_session(unsigned long long key) noexcept {
		};
		inline void do_send_session(unsigned long long key, message& message) noexcept {
			session& session_ = _session_array[key];
			if (session_.acquire(key)) {
				session_._send_queue.emplace(message);
				if (session_.send())
					return;
			}
			if (session_.release())
				_iocp.post(*this, 0, static_cast<uintptr_t>(task::destory), reinterpret_cast<OVERLAPPED*>(&session_));
		}
		inline static auto create_message(size_type size) noexcept -> message {
			message _message = pool::instance().allocate(size);
			header header_;
			header_._size = 8;
			_message.push(reinterpret_cast<unsigned char*>(&header_), sizeof(header));
			return _message;
		}
	};

}