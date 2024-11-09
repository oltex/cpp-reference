#pragma once
#include "../kernel/object.h"
#include "../../data-structure/tuple/tuple.h"
#include <Windows.h>

namespace input_output {
	class completion final : public kernel::object {
	public:
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
		inline void connect_port(HANDLE handle, ULONG_PTR key) noexcept {
			CreateIoCompletionPort(handle, _handle, key, 0);
		}
		inline auto get_queue_state(unsigned long milli_second) noexcept {
			DWORD byte;
			ULONG_PTR key;
			OVERLAPPED* overlapped;
			bool result = GetQueuedCompletionStatus(_handle, &byte, &key, &overlapped, milli_second);
		}
		inline void post_queue_state(unsigned long byte, uintptr_t key, OVERLAPPED* overlapped) noexcept {
			PostQueuedCompletionStatus(_handle, byte, key, overlapped);
		}
	};
}