#pragma once
#include "critical_section.h"
#include "slim_read_write_lock.h"
#include <Windows.h>

namespace multi {
	class condition_variable final {
	public:
		inline explicit condition_variable(void) noexcept {
			InitializeConditionVariable(&_condition_variable);
		}
		inline explicit condition_variable(condition_variable const& rhs) noexcept = delete;
		inline explicit condition_variable(condition_variable&& rhs) noexcept = delete;
		inline auto operator=(condition_variable const& rhs) noexcept -> condition_variable & = delete;
		inline auto operator=(condition_variable&& rhs) noexcept -> condition_variable & = delete;
		inline ~condition_variable(void) noexcept = default;
	public:
		inline void sleep(lock::critical_section& critical_section_, unsigned long milli_second) noexcept {
			SleepConditionVariableCS(&_condition_variable, &critical_section_.data(), milli_second);
		}
		inline void sleep(lock::slim_read_write& slim_read_write, unsigned long milli_second, unsigned long flag) noexcept {
			SleepConditionVariableSRW(&_condition_variable, &slim_read_write.data(), milli_second, flag);
		}
		inline void wake(void) noexcept {
			WakeConditionVariable(&_condition_variable);
		}
		inline void wake_all(void) noexcept {
			WakeAllConditionVariable(&_condition_variable);
		}
	private:
		CONDITION_VARIABLE _condition_variable;
	};
}