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

			_RIO_EXTENSION_FUNCTION_TABLE rio;
			GUID functionTableId = WSAID_MULTIPLE_RIO;  // RIO ÇÔ¼ö GUID
			DWORD bytesReturned = 0;
			if (SOCKET_ERROR == WSAIoctl(_socket.data(), SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER, &functionTableId, sizeof(GUID), &rio, sizeof(_RIO_EXTENSION_FUNCTION_TABLE), &bytesReturned, NULL, NULL)) {
				int a = 10;
			}
		}
	private:

	};
}