#pragma once
#include <ctime>
#include "time_struct.h"

namespace system_component::time {
	class unix_time final {
	public:
		inline explicit unix_time(void) noexcept = default;
		inline explicit unix_time(unix_time const& rhs) noexcept
			: _time_t(rhs._time_t) {
		}
		inline explicit unix_time(unix_time&& rhs) noexcept
			: _time_t(rhs._time_t) {
		}
		inline auto operator=(unix_time const& rhs) noexcept -> unix_time& {
			_time_t = rhs._time_t;
		}
		inline auto operator=(unix_time&& rhs) noexcept -> unix_time& {
			_time_t = rhs._time_t;
		}
		inline ~unix_time(void) noexcept = default;
	public:
		inline void time(void) noexcept {
			_time64(&_time_t);
		}
		inline auto local_time(void) const noexcept -> time_struct {
			std::tm tm;
			_localtime64_s(&tm, &_time_t);
			return time_struct(tm);
		}
	public:
		inline auto data(void) noexcept -> __time64_t& {
			return _time_t;
		}
	private:
		__time64_t _time_t;
	};
}