#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include "../system/socket/socket.h"
#include "../system/handle/handle.h"
#include "../data-structure/tuple/tuple.h"

//overlaped용 헤더
//#pragma once
////#include "../multi/event.h"
//#include <Windows.h>

//rio용 헤더
//#pragma once
//#pragma comment(lib,"ws2_32.lib")
//#include <WinSock2.h>
//#pragma comment(lib, "mswsock.lib")
//#include <MSWSock.h>
//#include <Windows.h>
//#include <Ws2def.h>
//#include "../socket/socket.h"


namespace library::input_output {
	class overlapped final {
	public:
		inline explicit overlapped(void) noexcept = default;
		inline explicit overlapped(overlapped const&) noexcept = delete;
		inline explicit overlapped(overlapped&&) noexcept = delete;
		inline auto operator=(overlapped const&) noexcept -> overlapped & = delete;
		inline auto operator=(overlapped&&) noexcept -> overlapped & = delete;
		inline ~overlapped(void) noexcept = default;

		//inline void set_event(multi::event& event) noexcept {
		//	_overlapped.hEvent = event.data();
		//}
		inline auto get_result(HANDLE handle, unsigned long* byte, bool const wait) noexcept -> bool {
			return GetOverlappedResult(handle, &_overlapped, byte, wait);
		}
		inline auto has_completed(void) const noexcept -> bool {
			return HasOverlappedIoCompleted(&_overlapped);
		}
		inline void clear(void) noexcept {
			memset(&_overlapped, 0, sizeof(_OVERLAPPED));
		}
		inline auto data(void) noexcept -> _OVERLAPPED& {
			return _overlapped;
		}
	private:
		_OVERLAPPED _overlapped;
	};

	class completion_port final : public system::handle {
	public:
		inline explicit completion_port(void) noexcept = default;
		inline explicit completion_port(unsigned long const concurrent_thread) noexcept
			: handle(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, concurrent_thread)) {
		};
		inline explicit completion_port(completion_port const&) noexcept = delete;
		inline explicit completion_port(completion_port&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(completion_port const&) noexcept -> completion_port & = delete;
		inline auto operator=(completion_port&& rhs) noexcept -> completion_port& {
			handle::operator=(std::move(rhs));
			return *this;
		}
		inline virtual ~completion_port(void) noexcept override = default;

		inline void create(unsigned long const concurrent_thread) noexcept {
			_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, concurrent_thread);
		}
		inline void connect(system::socket& socket, ULONG_PTR const key) noexcept {
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket.data()), _handle, key, 0);
		}
		struct get_queue_state_result final {
			bool _result;
			DWORD _transferred;
			ULONG_PTR _key;
			OVERLAPPED* _overlapped;
		};
		inline auto get_queue_state(unsigned long const milli_second) noexcept -> get_queue_state_result {
			get_queue_state_result result;
			result._result = GetQueuedCompletionStatus(_handle, &result._transferred, &result._key, &result._overlapped, milli_second);
			return result;
		}
		inline auto get_queue_state_tuple(unsigned long const milli_second) noexcept -> library::data_structure::tuple<bool, DWORD, ULONG_PTR, OVERLAPPED*> {
			library::data_structure::tuple<bool, DWORD, ULONG_PTR, OVERLAPPED*> result;
			result.get<0>() = GetQueuedCompletionStatus(_handle, &result.get<1>(), &result.get<2>(), &result.get<3>(), milli_second);
			return result;
		}
		//inline auto get_queue_state_ex(void) noexcept {
		//	GetQueuedCompletionStatusEx()
		//}
		inline void post_queue_state(unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept {
			PostQueuedCompletionStatus(_handle, transferred, key, overlapped);
		}
	};


	//	class registered_inputouput final {
	//	public:
	//		inline explicit registered_inputouput(void) noexcept {
	//			wsa_start_up();
	//			socket _socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//			GUID functionTableId = WSAID_MULTIPLE_RIO;  // RIO 함수 GUID
	//			DWORD bytesReturned = 0;
	//			if (SOCKET_ERROR == WSAIoctl(_socket.data(), SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER, &functionTableId, sizeof(GUID), &rio, sizeof(_RIO_EXTENSION_FUNCTION_TABLE), &bytesReturned, NULL, NULL)) {
	//				__debugbreak();
	//			}
	//			//RQ 2개 소켓당 소켓하나에서 recv send를 락걸필요가 없다
	//			RIO_MAX_RESULTS
	//				_rio_extension_function_table.RIOCreateRequestQueue()
	//		}
	//	private:
	//		_RIO_EXTENSION_FUNCTION_TABLE _rio_extension_function_table;
	//	};
}