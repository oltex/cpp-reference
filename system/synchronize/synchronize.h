#pragma once
#pragma comment(lib, "Synchronization.lib")
#include "../handle/handle.h"
#include <Windows.h>
#include <intrin.h>
#include <utility>
#define x64

namespace library {
	namespace barrier {
		inline void read(void) noexcept {
			_ReadBarrier();
		}
		inline void write(void) noexcept {
			_WriteBarrier();
		}
		inline void read_write(void) noexcept {
			_ReadWriteBarrier();
		}
	}
	namespace fence {
		inline void load(void) noexcept {
			_mm_lfence();
		}
		inline void store(void) noexcept {
			_mm_sfence();
		}
		inline void memory(void) noexcept {
			_mm_mfence();
		}
		inline void fast_store(void) noexcept {
#ifdef x64
			__faststorefence();
#else
			LONG Barrier;
			_InterlockedOr(&Barrier, 0);
#endif
		}
		inline void flush(void const* address) noexcept {
			_mm_clflush(address);
		}
	}

	class spin_lock final {
	public:
		inline explicit spin_lock(void) noexcept = default;
		inline explicit spin_lock(spin_lock const& rhs) noexcept = delete;
		inline explicit spin_lock(spin_lock&& rhs) noexcept = delete;
		inline auto operator=(spin_lock const& rhs) noexcept -> spin_lock & = delete;
		inline auto operator=(spin_lock&& rhs) noexcept -> spin_lock & = delete;
		inline ~spin_lock(void) noexcept = default;
	public:
		inline void lock(void) noexcept {
			while (1 == _InterlockedExchange(&_lock, 1))
				while (1 == _lock)
					YieldProcessor();
		}
		inline void unlock(void) noexcept {
			_InterlockedExchange(&_lock, 0);
			//_lock = 0;
		}
	public:
		volatile long _lock = 0;
	};
	namespace wait_on_address {
		inline bool wait(void* address, void* _compare, size_t const size, unsigned long const milli_second) noexcept {
			return WaitOnAddress(address, _compare, size, milli_second);
		}
		inline void wake_single(void* address) noexcept {
			WakeByAddressSingle(address);
		}
		inline void wake_all(void* address) noexcept {
			WakeByAddressAll(address);
		}
	}
	class wait_on_address_lock final {
	public:
		inline explicit wait_on_address_lock(void) noexcept = default;
		inline explicit wait_on_address_lock(wait_on_address_lock const& rhs) noexcept = delete;
		inline explicit wait_on_address_lock(wait_on_address_lock&& rhs) noexcept = delete;
		inline auto operator=(wait_on_address_lock const& rhs) noexcept -> wait_on_address_lock & = delete;
		inline auto operator=(wait_on_address_lock&& rhs) noexcept -> wait_on_address_lock & = delete;
		inline ~wait_on_address_lock(void) noexcept = default;

		inline void lock(void) noexcept {
			volatile long compare = 1;
			while (1 == _address || 1 == _InterlockedExchange(&_address, 1))
				WaitOnAddress(&_address, (void*)&compare, sizeof(long), INFINITE);
		}
		inline void unlock(void) noexcept {
			_address = 0;
			WakeByAddressSingle((void*)&_address);
		}
	private:
		volatile long _address = 0;
	};

	class critical_section final {
	public:
		inline explicit critical_section(void) noexcept {
			InitializeCriticalSection(&_critical_section);
		};
		inline explicit critical_section(critical_section const&) noexcept = delete;
		inline explicit critical_section(critical_section&&) noexcept = delete;
		inline auto operator=(critical_section const&) noexcept -> critical_section & = delete;
		inline auto operator=(critical_section&&) noexcept -> critical_section & = delete;
		inline ~critical_section(void) noexcept {
			DeleteCriticalSection(&_critical_section);
		};
	public:
		inline void enter(void) noexcept {
			EnterCriticalSection(&_critical_section);
		}
		inline bool try_enter(void) noexcept {
			return TryEnterCriticalSection(&_critical_section);
		}
		inline void leave(void) noexcept {
			LeaveCriticalSection(&_critical_section);
		}
	public:
		inline auto data(void) noexcept -> CRITICAL_SECTION& {
			return _critical_section;
		}
	private:
		CRITICAL_SECTION _critical_section;
	};
	class slim_read_write_lock final {
	public:
		inline explicit slim_read_write_lock(void) noexcept {
			InitializeSRWLock(&_srwlock);
		};
		inline explicit slim_read_write_lock(slim_read_write_lock const&) noexcept = delete;
		inline explicit slim_read_write_lock(slim_read_write_lock&&) noexcept = delete;
		inline auto operator=(slim_read_write_lock const&) noexcept -> slim_read_write_lock & = delete;
		inline auto operator=(slim_read_write_lock&&) noexcept -> slim_read_write_lock & = delete;
		inline ~slim_read_write_lock(void) noexcept = default;
	public:
		inline void acquire_exclusive(void) noexcept {
			AcquireSRWLockExclusive(&_srwlock);
		}
		inline bool try_acquire_exclusive(void) noexcept {
			return TryAcquireSRWLockExclusive(&_srwlock);
		}
		inline void acquire_shared(void) noexcept {
			AcquireSRWLockShared(&_srwlock);
		}
		inline bool try_acquire_shared(void) noexcept {
			return TryAcquireSRWLockShared(&_srwlock);
		}
		inline void release_exclusive(void) noexcept {
#pragma warning(suppress: 26110)
			ReleaseSRWLockExclusive(&_srwlock);
		}
		inline void release_shared(void) noexcept {
			ReleaseSRWLockShared(&_srwlock);
		}
	public:
		inline auto data(void) noexcept -> SRWLOCK& {
			return _srwlock;
		}
	private:
		SRWLOCK _srwlock;
	};

	class event final : public handle {
	public:
		inline explicit event(void) noexcept = default;
		inline explicit event(bool const manual, bool const initial_state) noexcept
			: handle(CreateEventW(nullptr, manual, initial_state, nullptr)) {
		};
		inline explicit event(event const&) noexcept = delete;
		inline explicit event(event&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(event const&) noexcept -> event & = delete;
		inline auto operator=(event&& rhs) noexcept -> event& {
			handle::operator=(std::move(rhs));
			return *this;
		};
		inline virtual ~event(void) noexcept override = default;

		inline void create(bool const manual, bool const initial_state) noexcept {
			_handle = CreateEventW(nullptr, manual, initial_state, nullptr);
		}
		inline void set(void) noexcept {
			SetEvent(_handle);
		}
		inline void reset(void) noexcept {
			ResetEvent(_handle);
		}
		inline void pulse(void) noexcept {
			PulseEvent(_handle);
		}
	};
	class mutex final : public handle {
	public:
		inline explicit mutex(bool const initial_owner) noexcept
			: handle(CreateMutexW(nullptr, initial_owner, nullptr)) {
		};
		inline explicit mutex(mutex const& rhs) noexcept = delete;
		inline explicit mutex(mutex&& rhs) noexcept
			: handle(std::move(rhs)) {
		};
		inline auto operator=(mutex const& rhs) noexcept -> mutex & = delete;
		inline auto operator=(mutex&& rhs) noexcept -> mutex& {
			handle::operator=(std::move(rhs));
		};
		inline virtual ~mutex(void) noexcept override = default;
	};
}