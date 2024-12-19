#pragma once
#include <iostream>
#include <iomanip>
#include <ctime>

namespace system_component::time {
	class time_struct final {
	public:
		inline explicit time_struct(void) noexcept = default;
		inline explicit time_struct(std::tm& tm) noexcept
			: _tm(tm) {
		};
		inline explicit time_struct(time_struct const& rhs) noexcept
			: _tm(rhs._tm) {
		};
		inline explicit time_struct(time_struct&& rhs) noexcept
			: _tm(rhs._tm) {
		}
		inline auto operator=(time_struct const& rhs) noexcept -> time_struct& {
			_tm = rhs._tm;
		}
		inline auto operator=(time_struct&& rhs) noexcept -> time_struct& {
			_tm = rhs._tm;
		}
		inline ~time_struct(void) noexcept = default;
	public:
		inline auto put_time(char const* const format) const noexcept {
			return std::put_time(&_tm, format);
		}
	private:
		std::tm _tm;
	};
}