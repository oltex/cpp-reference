#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include "../network/socket.h"
#include "../kernel/object.h"
#include "../../data-structure/tuple/tuple.h"

namespace input_output {
	class completion final : public kernel::object {
	public:
		inline explicit completion(void) noexcept = default;
		inline explicit completion(unsigned long const concurrent_thread) noexcept
			: object(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, concurrent_thread)) {
		};
		inline explicit completion(completion const& rhs) noexcept = delete;
		inline explicit completion(completion&& rhs) noexcept
			: object(std::move(rhs)) {
		};
		inline auto operator=(completion const& rhs) noexcept -> completion & = delete;
		inline auto operator=(completion&& rhs) noexcept -> completion& {
			object::operator=(std::move(rhs));
			return *this;
		}
		inline virtual ~completion(void) noexcept override = default;
	public:
		inline void create_port(unsigned long const concurrent_thread) noexcept {
			_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, concurrent_thread);
		}
		inline void connect_port(network::socket& socket, ULONG_PTR key) noexcept {
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket.data()), _handle, key, 0);
		}
		struct get_queue_state_result final {
			bool _result;
			DWORD _transferred;
			ULONG_PTR _key;
			OVERLAPPED* _overlapped;
		};
		inline auto get_queue_state(unsigned long milli_second) noexcept -> get_queue_state_result {
			get_queue_state_result result;
			result._result = GetQueuedCompletionStatus(_handle, &result._transferred, &result._key, &result._overlapped, milli_second);
			return result;
		}
		inline auto get_queue_state_tuple(unsigned long milli_second) noexcept -> data_structure::tuple<bool, DWORD, ULONG_PTR, OVERLAPPED*> {
			data_structure::tuple<bool, DWORD, ULONG_PTR, OVERLAPPED*> result;
			result.get<0>() = GetQueuedCompletionStatus(_handle, &result.get<1>(), &result.get<2>(), &result.get<3>(), milli_second);
			return result;
		}
		inline void post_queue_state(unsigned long byte, uintptr_t key, OVERLAPPED* overlapped) noexcept {
			PostQueuedCompletionStatus(_handle, byte, key, overlapped);
		}
	};
}