#pragma once
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")
#include <iostream>
#include <iomanip>
#include <ctime>

namespace library::time {
	namespace multimidia {
		inline void begin_period(unsigned int const peroid) noexcept {
			timeBeginPeriod(peroid);
		}
		inline void end_period(unsigned int const peroid) noexcept {
			timeEndPeriod(peroid);
		}
		inline auto get_time(void) noexcept -> unsigned long {
			return timeGetTime();
		}
	}
	inline auto get_tick_count(void) noexcept -> unsigned long {
#pragma warning(suppress: 28159)
		return GetTickCount();
	}
	inline auto get_tick_count64(void) noexcept -> unsigned long long {
		return GetTickCount64();
	}

	class unix final {
	public:
		inline explicit unix(void) noexcept = default;
		inline explicit unix(__time64_t time_t) noexcept
			: _time_t(time_t) {
		};
		inline explicit unix(unix const& rhs) noexcept
			: _time_t(rhs._time_t) {
		}
		inline explicit unix(unix&& rhs) noexcept
			: _time_t(rhs._time_t) {
		}
		inline auto operator=(unix const& rhs) noexcept -> unix& {
			_time_t = rhs._time_t;
		}
		inline auto operator=(unix&& rhs) noexcept -> unix& {
			_time_t = rhs._time_t;
		}
		inline ~unix(void) noexcept = default;

		inline void time(void) noexcept {
			_time64(&_time_t);
		}
		inline auto local_time(void) const noexcept -> tm {
			std::tm tm;
			_localtime64_s(&tm, &_time_t);
			return tm;
		}
		inline auto data(void) noexcept -> __time64_t& {
			return _time_t;
		}
	private:
		__time64_t _time_t;
	};
	class date final {
	public:
		inline explicit date(void) noexcept = default;
		inline explicit date(std::tm tm) noexcept
			: _tm(tm) {
		};
		inline explicit date(date const& rhs) noexcept
			: _tm(rhs._tm) {
		};
		inline explicit date(date&& rhs) noexcept
			: _tm(rhs._tm) {
		}
		inline auto operator=(date const& rhs) noexcept -> date& {
			_tm = rhs._tm;
		}
		inline auto operator=(date&& rhs) noexcept -> date& {
			_tm = rhs._tm;
		}
		inline ~date(void) noexcept = default;

		inline auto put_time(char const* const format) const noexcept {
			return std::put_time(&_tm, format);
		}
		inline auto year(void) const noexcept -> int {
			return _tm.tm_year - 100;
		}
		inline auto month(void) const noexcept -> int {
			return _tm.tm_mon + 1;
		}
		inline auto day_of_year(void) const noexcept -> int {
			return _tm.tm_yday + 1;
		}
		inline auto day_of_month(void) const noexcept -> int {
			return _tm.tm_mday;
		}
		inline auto day_of_week(void) const noexcept -> int {
			return _tm.tm_wday;
		}
		inline auto hour(void) const noexcept -> int {
			return _tm.tm_hour;
		}
		inline auto minute(void) const noexcept -> int {
			return _tm.tm_min;
		}
		inline auto second(void) const noexcept -> int {
			return _tm.tm_sec;
		}
	private:
		std::tm _tm;
	};
}

namespace library::system::time {
	class query_performance final {
	public:
		inline explicit query_performance(void) noexcept = default;
		inline explicit query_performance(LARGE_INTEGER large_integer) noexcept
			: _large_integer(large_integer) {
		}
		inline explicit query_performance(LONGLONG quad_part) noexcept {
			_large_integer.QuadPart = quad_part;
		}
		inline explicit query_performance(query_performance const& rhs) noexcept
			: _large_integer(rhs._large_integer) {
		};
		inline explicit query_performance(query_performance&& rhs) noexcept
			: _large_integer(rhs._large_integer) {
		}
		inline auto operator=(query_performance const& rhs) noexcept -> query_performance& {
			_large_integer = rhs._large_integer;
			return *this;
		}
		inline auto operator=(query_performance&& rhs) noexcept -> query_performance& {
			_large_integer = rhs._large_integer;
			return *this;
		}
		inline ~query_performance(void) noexcept = default;

		inline void counter(void) noexcept {
			QueryPerformanceCounter(&_large_integer);
		}
		inline void frequency(void) noexcept {
			QueryPerformanceFrequency(&_large_integer);
		}
		inline void clear(void) noexcept {
			_large_integer.QuadPart = 0;
		}
		inline auto operator+(query_performance const& rhs) noexcept -> query_performance {
			return query_performance(_large_integer.QuadPart + rhs._large_integer.QuadPart);
		}
		inline auto operator+=(query_performance const& rhs) noexcept ->query_performance& {
			_large_integer.QuadPart += rhs._large_integer.QuadPart;
			return *this;
		}
		inline auto operator-(query_performance const& rhs) noexcept ->query_performance {
			return query_performance(_large_integer.QuadPart - rhs._large_integer.QuadPart);
		}
		inline auto operator-=(query_performance const& rhs) noexcept ->query_performance& {
			_large_integer.QuadPart -= rhs._large_integer.QuadPart;
			return *this;
		}
		inline auto operator/(query_performance const& rhs) noexcept ->query_performance {
			return query_performance(_large_integer.QuadPart / rhs._large_integer.QuadPart);
		}
		inline auto operator/=(query_performance const& rhs) noexcept ->query_performance& {
			_large_integer.QuadPart /= rhs._large_integer.QuadPart;
			return *this;
		}
		inline auto data(void) noexcept -> LARGE_INTEGER& {
			return _large_integer;
		}
	private:
		LARGE_INTEGER _large_integer;
	};
	//inline static query_performance _frequency();
}
