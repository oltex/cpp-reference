#pragma once
#include "module/scheduler/io_complet_port.h"
#include "network.h"
#include "session.h"
#include "monitor.h"

namespace framework {
	class server final : public io_complet_port::io_complet_object {
		using size_type = unsigned int;
		enum class task : unsigned char {
			accept = 0, connect, session, destory
		};
		network _network;
		session_array _session_array;
		monitor _monitor;
	public:
		inline explicit server(size_type sessions) noexcept
			: _session_array(sessions) {
			_iocp.execute(&server::monitor, this);
			_iocp.execute(&server::timeout, this);
		}
		inline explicit server(server const&) noexcept = delete;
		inline explicit server(server&&) noexcept = delete;
		inline auto operator=(server const&) noexcept -> server & = delete;
		inline auto operator=(server&&) noexcept -> server & = delete;
		inline ~server(void) noexcept = default;

		inline void start_listen(char const* const ip, unsigned short port, int backlog) noexcept {
			_network.listen(ip, port, backlog);
			_iocp.connect(*this, _network._listen, static_cast<uintptr_t>(task::accept));
			_network.accept(16);
		}
		inline void stop_listen(void) noexcept {
			_network.close();
		}
		inline void connect(char const* const ip, unsigned short port) noexcept {
			_network.connect();
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
		inline virtual void worker(bool result, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept override {
			switch (auto task = static_cast<server::task>(key)) {
			case task::accept:
			case task::connect: {
				auto& connection = connection::recover(overlapped);
				if (false == result)
					connection.close();
				else {
					connection.inherit(_network);
					library::socket_address_ipv4 address;
					if (task::accept == task)
						address = connection.address();
					else
						int a = 10; //주소 구하기
					if (true == accept_socket(address)) {
						auto session = _session_array.allocate(connection, 40000, 40000);
						if (nullptr != session) {
							_iocp.connect(*this, session->_socket, static_cast<uintptr_t>(task::session));
							create_session(session->_key);
							if (!session->receive() && session->release<false>()) {
								destroy_session(session->_key);
								_session_array.deallocate(session);
							}
						}
					}
					if (task::accept == task)
						_network.accept(connection);
				}
			} break;
			case task::session: {
				auto& session = _session_array[overlapped];
				if (0 != transferred) {
					if (&session._receive_overlap.data() == overlapped) {
						session._receive_message.move_rear(transferred);
						session.receive_timeout();

						while (auto message = session.receive_message()) {
							if (false == receive_session(session._key, *message)) {
								session.cancel();
								break;
							}
						}
						if (session.receive())
							break;
					}
					else {
						session.send_finish();
						if (session.send())
							break;
					}
				}
				bool flag;
				if (&session._receive_overlap.data() == overlapped)
					flag = session.release<false>();
				else
					flag = session.release<true>();
				if (flag) {
					destroy_session(session._key);
					_session_array.deallocate(&session);
				}
			} break;
			case task::destory: {
				session& session = *reinterpret_cast<framework::session*>(overlapped);
				on_destroy_session(session._key);
				_session_array.deallocate(&session);
			} break;
			}
		};
		inline int monitor(void) noexcept {
			_monitor._query.collect_query_data();
			printf("--------------------------------------\n"\
				"[ System Monitor ]\n"\
				"CPU Usage\n"\
				"          System     Process \n"\
				" Total  : %7.3f %%  %7.3f %%\n"\
				" User   : %7.3f %%  %7.3f %%\n"\
				" Kernel : %7.3f %%  %7.3f %%\n"\
				"Memory Usage\n"\
				"             System      Process \n"\
				" Available : %7.3f GB\n"\
				" Private   :             %7.3f MB\n"\
				" Non-Page  : %7.3f MB  %7.3f MB\n"\
				"Network Usage\n"\
				"              Volume      Segment \n"\
				" Receive    : %7.3f MB %8d\n"\
				" Send       : %7.3f MB %8d\n"
				" Retransmit :            %8d\n",
				_monitor._system_total_time.get_format_value<double>(),
				_monitor._system_user_time.get_format_value<double>(),
				_monitor._system_kernel_time.get_format_value<double>(),
				_monitor._process_total_time.get_format_value<double>(PDH_FMT_NOCAP100) / 8,
				_monitor._process_user_time.get_format_value<double>(PDH_FMT_NOCAP100) / 8,
				_monitor._process_kernel_time.get_format_value<double>(PDH_FMT_NOCAP100) / 8,
				_monitor._system_available_memory.get_format_value<long long>() / static_cast<double>(0x40000000),
				_monitor._system_nonpage_memory.get_format_value<long long>() / static_cast<double>(0x100000),
				_monitor._process_private_memory.get_format_value<long long>() / static_cast<double>(0x100000),
				_monitor._process_nonpage_memory.get_format_value<long long>() / static_cast<double>(0x100000),
				_monitor._network_receive.get_format_value<double>() / static_cast<double>(0x100000), _monitor._segment_receive.get_format_value<long>(),
				_monitor._network_send.get_format_value<double>() / static_cast<double>(0x100000), _monitor._segment_send.get_format_value<long>(),
				_monitor._segment_retransmit.get_format_value<long>());

			
			printf("--------------------------------------\n"\
				"[ Server Monitor ]\n"\
				"IOCP Usage\n"\
				"            Rate         Total\n"\
				" Accept  : %7u TPS  %7llu \n"\
				" Receive : %7u TPS\n"
				" Send    : %7u TPS\n"
				"Resource Usage\n"\
				" Session : %8u\n"\
				" Message : %8u\n",
				_monitor._accept_tps, _monitor._accept_total,
				_monitor._receive_tps, _monitor._send_tps,
				_session_array.size(), pool::size());

			library::interlock_exchange(_monitor._accept_tps, 0);
			library::interlock_exchange(_monitor._receive_tps, 0);
			library::interlock_exchange(_monitor._send_tps, 0);
			return 1000;
		}
		inline int timeout(void) noexcept {
			for (auto iter = _session_array.begin(), end = _session_array.end(); iter != end; ++iter) {
				if (iter->acquire()) {
					if (library::get_tick_count64() > iter->_receive_expire || library::get_tick_count64() > iter->_send_expire) {
						iter->cancel();
					}
				}
				if (iter->release()) {
					destroy_session(iter->_key);
					_session_array.deallocate(&*iter);
				}
			}
			return 1000;
		}
	public:
		inline virtual bool accept_socket(library::socket_address_ipv4& socket_address) noexcept {
			return true;
		}
		inline virtual void create_session(unsigned long long key) noexcept {
			//message_pointer message_ = server::create_message();
			//*message_ << 0x7fffffffffffffff;
			//do_send_session(key, message_);
		}
		inline virtual bool receive_session(unsigned long long key, message& message) noexcept {
			unsigned long long value;
			message >> value;
			auto message_ = create_message(10);
			message_ << value;

			send_session(key, message_);
			return true;
		};
		inline virtual void destroy_session(unsigned long long key) noexcept {
		};
		inline void send_session(unsigned long long key, message& message) noexcept {
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
			message _message(pool::instance().allocate(sizeof(header) + size));
			header header_;
			header_._size = 8;
			_message.push(reinterpret_cast<char*>(&header_), sizeof(header));
			return _message;
		}
	};
}