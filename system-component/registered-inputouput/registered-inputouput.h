#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#pragma comment(lib, "mswsock.lib")
#include <MSWSock.h>

#include <Windows.h>
#include <Ws2def.h>

#include "../socket/socket.h"

namespace system_component {
	class registered_inputouput final {
	public:
		inline explicit registered_inputouput(void) noexcept {
			wsa_start_up();
			socket _socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			GUID functionTableId = WSAID_MULTIPLE_RIO;  // RIO 함수 GUID
			DWORD bytesReturned = 0;
			if (SOCKET_ERROR == WSAIoctl(_socket.data(), SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER, &functionTableId, sizeof(GUID), &rio, sizeof(_RIO_EXTENSION_FUNCTION_TABLE), &bytesReturned, NULL, NULL)) {
				__debugbreak();
			}
			//RQ 2개 소켓당 소켓하나에서 recv send를 락걸필요가 없다
			RIO_MAX_RESULTS
			_rio_extension_function_table.RIOCreateRequestQueue()
		}
	private:
		_RIO_EXTENSION_FUNCTION_TABLE _rio_extension_function_table;
	};
}