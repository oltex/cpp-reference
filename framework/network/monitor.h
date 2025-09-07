#pragma once
#include "module/utility/performance_data_help.h"

namespace framework {
	struct monitor final {
		library::pdh_query& _query;
		library::pdh_counter& _system_total_time;
		library::pdh_counter& _system_user_time;
		library::pdh_counter& _system_kernel_time;
		library::pdh_counter& _process_total_time;
		library::pdh_counter& _process_user_time;
		library::pdh_counter& _process_kernel_time;
		library::pdh_counter& _system_available_memory;
		library::pdh_counter& _system_nonpage_memory;
		library::pdh_counter& _process_private_memory;
		library::pdh_counter& _process_nonpage_memory;
		library::pdh_counter& _network_receive;
		library::pdh_counter& _network_send;
		library::pdh_counter& _segment_receive;
		library::pdh_counter& _segment_send;
		library::pdh_counter& _segment_retransmit;

		unsigned long long _accept_total = 0;
		unsigned long _accept_tps = 0;
		unsigned long _receive_tps = 0;
		unsigned long _send_tps = 0;
		//unsigned long long _timeout_total = 0

		inline explicit monitor(void) noexcept
			: _query(library::pdh_query::instance()),
			_system_total_time(_query.add_counter(L"\\Processor(_Total)\\% Processor Time")),
			_system_user_time(_query.add_counter(L"\\Processor(_Total)\\% User Time")),
			_system_kernel_time(_query.add_counter(L"\\Processor(_Total)\\% Privileged Time")),
			_process_total_time(_query.add_counter(L"\\Process(network)\\% Processor Time")),
			_process_user_time(_query.add_counter(L"\\Process(network)\\% User Time")),
			_process_kernel_time(_query.add_counter(L"\\Process(network)\\% Privileged Time")),
			_system_available_memory(_query.add_counter(L"\\Memory\\Available Bytes")),
			_system_nonpage_memory(_query.add_counter(L"\\Memory\\Pool Nonpaged Bytes")),
			_process_private_memory(_query.add_counter(L"\\Process(network)\\Private Bytes")),
			_process_nonpage_memory(_query.add_counter(L"\\Process(network)\\Pool Nonpaged Bytes")),
			_network_receive(_query.add_counter(L"\\Network Interface(*)\\Bytes Received/sec")),
			_network_send(_query.add_counter(L"\\Network Interface(*)\\Bytes Sent/sec")),
			_segment_receive(_query.add_counter(L"\\TCPv4\\Segments Received/sec")),
			_segment_send(_query.add_counter(L"\\TCPv4\\Segments Sent/sec")),
			_segment_retransmit(_query.add_counter(L"\\TCPv4\\Segments Retransmitted/sec")) {
		};
		inline explicit monitor(monitor const&) noexcept = delete;
		inline explicit monitor(monitor&&) noexcept = delete;
		inline auto operator=(monitor const&) noexcept -> monitor & = delete;
		inline auto operator=(monitor&&) noexcept -> monitor & = delete;
		inline ~monitor(void) noexcept = default;

		inline void update_session(bool receive) noexcept {
			if (receive)
				library::interlock_increment(_receive_tps);
			else
				library::interlock_increment(_send_tps);
		}
	};
}