#pragma once
#include "module/scheduler/io_complet_port.h"
#include "connection.h"
#include "session.h"
#include "monitor.h"

namespace framework {
	class server final : public io_complet_port::io_complet_object {
		using size_type = unsigned int;
		enum class task : unsigned char {
			accept = 0, connect, session, destory
		};
		connection _listen;
		connection_array _connection_array;
		session_array _session_array;
		monitor _monitor;
	public:
		inline explicit server(size_type connection, size_type session) noexcept
			: _connection_array(connection), _session_array(session) {
			_iocp.execute(&server::monitor, this);
			_iocp.execute(&server::timeout, this);
		}
		inline explicit server(server const&) noexcept = delete;
		inline explicit server(server&&) noexcept = delete;
		inline auto operator=(server const&) noexcept -> server & = delete;
		inline auto operator=(server&&) noexcept -> server & = delete;
		inline ~server(void) noexcept = default;

		inline void start_listen(char const* const ip, unsigned short port, int backlog) noexcept {
			_listen.listen_socket(ip, port, backlog);
			_iocp.connect(*this, _listen._socket, static_cast<uintptr_t>(task::accept));
			for (auto index = 0; index < 8; ++index) {
				auto connection = _connection_array.allocate();
				connection->create_socket();
				if(!_listen.accept_post(*connection))
					_connection_array.deallocate(connection);
			}
		}
		inline void stop_listen(void) noexcept {
			_listen.close_socket();
			//_network.listen_stop();
		}
		inline void connect_socket(char const* const ip, unsigned short port) noexcept {
			if (auto connection = _connection_array.allocate(); nullptr != connection) {
				connection->create_socket();
				_iocp.connect(*this, connection->_socket, static_cast<uintptr_t>(task::connect));
				if (!connection->connect_post(ip, port))
					_connection_array.deallocate(connection);
			}
		}
		inline virtual bool create_socket(char const* const ip, unsigned short port) noexcept {
			return true;
		}
		inline virtual void create_session(unsigned long long key) noexcept {
			//message_pointer message_ = server::create_message();
			//*message_ << 0x7fffffffffffffff;
			//do_send_session(key, message_);
		}
		inline virtual bool receive_session(unsigned long long key, framework::message& message) noexcept {
			unsigned long long value;
			message >> value;
			auto message_ = create_message(10);
			message_ << value;

			send_session(key, message_);
			return true;
		};
		inline void send_session(unsigned long long key, framework::message& message) noexcept {
			auto& session = _session_array[key];
			if (session.acquire_iocount(key))
				if (session.send_message(message))
					return;
			if (session.release_iocount(false))
				_iocp.post(*this, 0, static_cast<uintptr_t>(task::destory), reinterpret_cast<OVERLAPPED*>(&session));
		}
		inline void close_session(unsigned long long key) noexcept {
			auto& session = _session_array[key];
			if (session.acquire_iocount(key))
				session.cancel_network();
			if (session.release_iocount(false))
				_iocp.post(*this, 0, static_cast<uintptr_t>(task::destory), reinterpret_cast<OVERLAPPED*>(&session));
		}
		inline virtual void destroy_session(unsigned long long key) noexcept {
		};
		inline static auto create_message(size_type size) noexcept -> framework::message {
			framework::message message(message_pool::instance().allocate(sizeof(header) + size));
			framework::header header{ ._size = 8 };
			message.push(reinterpret_cast<char*>(&header), sizeof(header));
			return message;
		}
	private:
		inline virtual void worker(bool result, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept override {
			switch (auto task = static_cast<server::task>(key)) {
			case task::accept:
			case task::connect: {
				auto& connection = connection::recover(overlapped);
				if (false != result) {
					library::socket_address_ipv4 address;
					if (task == task::accept) {
						connection.accept_inherit(_listen);
						address = connection.accept_address();
					}
					else {
						_listen.connect_inherit(connection);
						address = connection.connect_address();
					}
					if (true == create_socket(address.ip().data(), address.port())) {
						if (auto session = _session_array.allocate(); nullptr != session) {
							session->initialize(connection, 40000, 40000);
							_iocp.connect(*this, session->_socket, static_cast<uintptr_t>(task::session));
							create_session(session->_key);
							bool flag = true;
							if (!session->receive_post(flag) && session->release_iocount(flag)) {
								destroy_session(session->_key);
								_session_array.deallocate(session);
							}
						}
					}
				}
				if (task == task::accept) {
					connection.close_socket();
					connection.create_socket();
					if (_listen.accept_post(connection))
						break;
				}
				_connection_array.deallocate(&connection);
			} break;
			case task::session: {
				auto& session = _session_array[overlapped];
				bool flag = &session._receive_overlap.data() == overlapped;
				if (0 != transferred) {
					if (flag) {
						session.receive_ready(transferred);
						while (auto message = session.receive_message())
							if (false == receive_session(session._key, *message)) {
								session.cancel_network();
								break;
							}
						if (session.receive_post(flag))
							break;
					}
					else {
						session.send_finish();
						if (session.send_post())
							break;
					}
				}
				if (session.release_iocount(flag)) {
					destroy_session(session._key);
					_session_array.deallocate(&session);
				}
			} break;
			case task::destory: {
				session& session = *reinterpret_cast<framework::session*>(overlapped);
				destroy_session(session._key);
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
				_session_array.size(), message_pool::size());

			library::interlock_exchange(_monitor._accept_tps, 0);
			library::interlock_exchange(_monitor._receive_tps, 0);
			library::interlock_exchange(_monitor._send_tps, 0);
			return 1000;
		}
		inline int timeout(void) noexcept {
			for (auto iter = _session_array.begin(), end = _session_array.end(); iter != end; ++iter) {
				if (iter->acquire_iocount()) {
					if (library::get_tick_count64() > iter->_receive_expire || library::get_tick_count64() > iter->_send_expire)
						iter->cancel_network();
				}
				if (iter->release_iocount(false)) {
					destroy_session(iter->_key);
					_session_array.deallocate(&*iter);
				}
			}
			return 1000;
		}
	};
}