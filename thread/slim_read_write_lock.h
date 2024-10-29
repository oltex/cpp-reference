#pragma once
#include <Windows.h>

class slim_read_write_lock final {
public:
	inline explicit slim_read_write_lock(void) noexcept {
		InitializeSRWLock(&_srwlock);
	};
	inline explicit slim_read_write_lock(slim_read_write_lock const& rhs) noexcept = delete;
	inline explicit slim_read_write_lock(slim_read_write_lock&& rhs) noexcept = delete;
	inline auto operator=(slim_read_write_lock const& rhs) noexcept -> slim_read_write_lock & = delete;
	inline auto operator=(slim_read_write_lock&& rhs) noexcept -> slim_read_write_lock & = delete;
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