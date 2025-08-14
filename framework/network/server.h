#pragma once
#include "iocp.h"
#include "listen.h"

#include "session.h"
#include "header.h"

namespace framework {
	class server final : iocp::object {
		enum class task {
			accept = 0, session
		};
		iocp& _iocp;
		listen _listen;

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
			_listen.initialize(ip, port, backlog);
			_iocp.connect(*this, _listen._socket, static_cast<uintptr_t>(task::accept));
			_listen.accept(16);
		}
		inline void reject(void) {
			_listen.finalize();
		}
		inline void connect(void) noexcept {

		}
	private:
		inline void worker(bool result, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept override {
			switch (static_cast<task>(key)) {
			case task::accept: {
				auto& accept = accept::recover(overlapped);
				if (false == result)
					accept._socket.close();
				else {
					auto address = accept.initialize(_listen);
					if (true == on_accept_socket(address)) {
						auto& session = *_session_array.allocate();
						session.initialize(accept, 400000);
						_iocp.connect(*this, session._socket, static_cast<uintptr_t>(task::session));

						on_create_session(session._key);
						if (!session.receive() && session.release<true>()) {
							on_destroy_session(session._key);
							_session_array.deallocate(&session);
						}
					}
					_listen.accept(accept);
				}
			} break;
			case task::session: {
				auto [session, task] = session::recover(overlapped);
				if (session::task::recv == task) {
					if (0 != transferred) {
						session._message.move_rear(transferred);
						for (;;) {
							auto message = session.message();
							if (!message)
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
						//_interlockedadd((volatile long*)&_send_tps, session_._send_size);
						//session_.finish_send();
						//session_.send()
					}
					if (session.release<true>()) {
						on_destroy_session(session._key);
						_session_array.deallocate(&session);
					}
				}
			} break;
			default:
				break;
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
			auto message_ = create_message();
			message_ << value;
			return true;
		};
		inline virtual void on_destroy_session(unsigned long long key) noexcept {
		};
		inline void do_send_session(unsigned long long key, message& message) noexcept {
			session& session_ = _session_array[key];
			if (session_.acquire(key)) {
				session_._send_queue.push(view_ptr);
				if (0 == _send_frame && session_.send())
					return;
			}
			if (session_.release())
				_complation_port.post_queue_state(0, static_cast<uintptr_t>(post_queue_state::destory_session), reinterpret_cast<OVERLAPPED*>(&session_));
		}

		inline static auto create_message(void) noexcept -> message {
			message _message;
			_message.initialize();

			header header_;
			header_._size = 8;
			_message.push(reinterpret_cast<unsigned char*>(&header_), sizeof(header));
			return _message;
		}
	};

}